#include<iostream>
#include<string>
#include<math.h>
#include<iomanip>
#include<algorithm>
#include<thread>
#include<atomic>
#include<mutex>
using namespace std;
int A[10];      //A[i]��¼��i+1��վʱ�Ŀ���λ��
int d[9];    //d[i]��¼i+1��i+2վ�ľ���
int num = 0;                 //num������¼��ǰ����Ʊ������
mutex mutx;
struct Person {
	char name[10];   //����
	char id[20];	//���֤��
	int f;			//��ʼվ��
	int l;			//Ŀ��վ��
	int pay;		//��Ҫ����Ǯ
};
Person p[20];
int find_min(int f,int l) {            //����վ������λ����С��ֵ���ڴ�fվ��lվֻ��min��Ʊ
	int min;
	min = A[f - 1];
	for (int i = f - 1; i < l - 1; i++) {
		if (min > A[i])
			min = A[i];
	}
	return min;
}
void buy_ticket(int f, int l) {  //��Ʊ�󣬴���ʼվ�㿪ʼ������ÿ��վ����λ-1ֱ��Ŀ��վ�㣬Ŀ��վ����λ��������Ҫ�³���
	int f1 = f, l1 = l;
	while (f1 != l1) {
		A[f1-1]--;
		f1++;
	}
}
void refund_ticket(int i) {   ///��Ʊ
	num--;                    //��Ʊ����-1
	int f1 = p[i].f;
	int l1 = p[i].l;
	while (f1 != l1) {
		A[f1 - 1]++;
		f1++;
	}
	while (p[i].f != 0) {     //������ǰ�ƶ�
		p[i] = p[i + 1];
		i++;
	}
}
int check(int f, int l) {
	if (f < 1 || l>10||f>9||l<2) {
		return -1;
	}
	int f1 = f, l1 = l;
	int seat = find_min(f, l);                //��¼��λ����
	return seat;
}
int need_pay(int f, int l) {
	int sum = 0;
	for (int i = f; i < l; i++)
		sum += d[i-1];
	return sum;
}

void  action(int th) {
	{
		this_thread::sleep_for(std::chrono::milliseconds(100));
		lock_guard<std::mutex> lck(mutx);
		int function;
		cout << endl<<endl<<"��ѡ���ܣ�" << endl << "1.��ѯ��Ʊ/��Ʊ     2.��ѯ��Ʊ      3.��Ʊ" << endl;
		cin >> function;
		if (function == 1) {
			int f, l;
			cout << "��������ʼվ�㣺";
			cin >> f;
			cout << "�����뵽��վ�㣺";
			cin >> l;
			int flag = check(f, l);
			if (flag > 0) {
				cout << "��" << flag << "��������λ����ǮΪ" << need_pay(f, l) << "Ԫ��" << endl;
			}
			else if (flag == -1) {
				cout << "����վ��Ƿ���" << endl;
				cout << "-----------------------" << endl;
				cout << "�߳�" << th << "ִ�����" << endl;
				cout << "-----------------------" << endl;
				return;
			}
			else {
				cout << "��Ǹ��û�п������λ�ˣ�";
				cout << "-----------------------" << endl;
				cout << "�߳�" << th << "ִ�����" << endl;
				cout << "-----------------------" << endl;
				return;
			}
			cout << "ѡ��Ʊ������1,����Ʊ������0����" << endl;
			cin >> flag;
			if (flag) {
				cout << "������������";
				cin >> p[num].name;				//num�ڽ����ѭ��ǰ����Ϊ0
				cout << "���������֤�ţ�";
				cin >> p[num].id;
				p[num].f = f;
				p[num].l = l;
				p[num].pay = need_pay(f, l);
				buy_ticket(f, l);      //����buy_ticket������Ʊ
				num++;                 //��Ʊ����+1
			}
			else {
				cout << "-----------------------" << endl;
				cout << "�߳�" << th << "ִ�����" << endl;
				cout << "-----------------------" << endl;
				return;
			}
				
		}
		else if (function == 2) {
			int function2;
			cout << "��ѡ���ѯ��Ʊ�ķ�ʽ��" << endl << "1.����    2.���֤��" << endl;
			cin >> function2;
			if (function2 == 1) {
				cout << "������������" << endl;
				char name[20];
				cin >> name;
				for (int i = 0; i < 20; i++) {
					if (strcmp(name, p[i].name) == 0) {
						cout << "--------------------" << endl;
						cout << "������" << p[i].name << "     ���֤�ţ�" << p[i].id << "     ��ʼվ��" << p[i].f << "     �յ�վ��" << p[i].l << "     Ʊ�ۣ�" << p[i].pay << endl;
						cout << "--------------------" << endl;
						break;
					}
					else if (i == 19)
						cout << "û�в�ѯ����" << endl;     //���������û�з��ּ�¼
				}
			}
			else if (function2 == 2) {
				cout << "���������֤�ţ�" << endl;
				char id[20];
				cin >> id;
				for (int i = 0; i < 20; i++) {
					if (strcmp(id, p[i].id) == 0) {
						cout << "��ѯ��������£�" << endl;
						cout << "--------------------" << endl;
						cout << "������" << p[i].name << "     ���֤�ţ�" << p[i].id << "     ��ʼվ��" << p[i].f << "     �յ�վ��" << p[i].l << "     Ʊ�ۣ�" << p[i].pay << endl;
						cout << "--------------------" << endl;
						break;
					}
					else if (i == 19)
						cout << "û�в�ѯ����" << endl;     //���������û�з��ּ�¼
				}
			}
		}
		else if (function == 3) {
			int function3;
			cout << "��ѡ����Ʊ�ķ�ʽ��" << endl << "1.����    2.���֤��" << endl;
			cin >> function3;

			if (function3 == 1) {
				cout << "������������" << endl;
				char name[20];
				cin >> name;
				for (int i = 0; i < 20; i++) {
					if (strcmp(name, p[i].name) == 0) {
						refund_ticket(i);
						cout << "��Ʊ�ɹ�" << "num====" << num << endl;                 //��Ʊ
						break;
					}
					else if (i == 19)
						cout << "δ��ѯ���������µĻ�Ʊ��" << endl;
				}
			}
			else if (function3 == 2) {
				cout << "���������֤�ţ�" << endl;
				char id[20];
				cin >> id;
				for (int i = 0; i < 20; i++) {
					if (strcmp(id, p[i].id) == 0) {
						refund_ticket(i);
						cout << "��Ʊ�ɹ�" << "num====" << num << endl;
						break;
					}
					//��Ʊ
					else if (i == 19)
						cout << "δ��ѯ�������֤���µĻ�Ʊ��" << endl;
				}
			}

		}
		cout << "-----------------------" << endl;
		cout << "�߳�" << th << "ִ�����" << endl;
		cout << "-----------------------" << endl;
		/*cout << "�Ƿ���ʾ��ʱ��λ�����" << endl;
		int kkkk;
		cin >> kkkk;
		if (kkkk == 1) {
			cout << "-----------------" << endl;
			for (int i = 0; i < 10; i++) {
				cout << i + 1 << "��վ��";
				cout << A[i] << "   " << endl;
			}
			cout << "-----------------" << endl;
		}*/
		/*else if (function == 3) {

			}*/
	}
}
int main() {
	for (int i = 0; i < 10; i++) {
		A[i] = 10;                            //��ʼ����λ����
	}
	for (int i = 0; i < 9; i++) {
		d[i] = (i*i+32) % 12+1;               //����վ������  ���۸�
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 30; j++) {
			if (i == 2 && j == 6) {
				cout << " ��ģ�⹺Ʊϵͳ ";
				j += 18;
			}
			cout << '-';
		}
		cout << endl;
	}
	cout << endl;
	memset(p, 0, sizeof(p));
	thread th1(action, 1);
	thread th2(action, 2);
	thread th3(action, 3);
	thread th4(action, 4);
	thread th5(action, 5);
	thread th6(action, 6);
	thread th7(action, 7);
	thread th8(action, 8);
	th1.join();
	th2.join();
	th3.join();
	th4.join();
	th5.join();
	th6.join();
	th7.join();
	th8.join();
	return 0;
}





