#include<iostream>
#include<string>
#include<math.h>
#include<iomanip>
#include<algorithm>
using namespace std;
int A[10];      //A[i]��¼��i��վʱ�ÿ���λ��
int d[9];    //d[i]��¼i+1��i+2վ�þ���
struct Person {
	char name[10];
	char id[20];
	int f;
	int l;
	int pay;
};
void buy_ticket(int f, int l) {
	int f1 = f, l1 = l;
	while (f1 != l1) {
		A[f1-1]--;
		f1++;
	}
}
int check(int f, int l) {
	int f1 = f, l1 = l;
	int flag = 1;
	while (f1 != l1) {
		if (A[f1] > 0)
			f1++;
		else {
			flag = 0;  //������վ��û����λ
			break;
		}
	}
	return flag;
}
int need_pay(int f, int l) {
	int sum = 0;
	for (int i = f; i < l; i++)
		sum += d[i-1];
	return sum;
}
int main() {
	for (int i = 0; i < 10; i++) {
		A[i] = 10;
	}
	for (int i = 0; i < 9; i++) {
		d[i] = (i + 56) % 12;
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
	Person p[20];
	memset(p, 0, sizeof(p));
	int num = 0;
	while (true) {
		int function;
		cout << "��ѡ���ܣ�" << endl << "1.��ѯ��Ʊ/��Ʊ     2.��ѯ��Ʊ      3.��Ʊ" << endl;
		cin >> function;
		if (function == 1) {
			int f, l;
			cout << "��������ʼվ�㣺";
			cin >> f;
			cout << "�����뵽��վ�㣺";
			cin >> l;
			int flag = check(f, l);
			if (flag) {
				cout << "�п�����λ����ǮΪ" << need_pay(f, l) << "Ԫ��" << endl;
			}
			else {
				cout << "��Ǹ��û�п������λ�ˣ�";
				continue;
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
				buy_ticket(f, l);      //����buy������Ʊ
				num++;
			}
			else
				continue;
			/*cout << "-----------------" << endl;
			for (int i = 0; i < 10; i++)
				cout << A[i] << "   " << endl;
			cout << "-----------------" << endl;*/
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
					if (strcmp(name,p[i].name)==0) {
						cout << "--------------------" << endl;
						cout << "������" << p[i].name << "     ���֤�ţ�" << p[i].id << "     ��ʼվ��" << p[i].f << "     �յ�վ��" << p[i].l << "     Ʊ�ۣ�" << p[i].pay << endl;
						cout << "--------------------" << endl;
						break;
					}
					if (i == 19)
						cout << "û�в�ѯ����" << endl;     //���������û�з��ּ�¼
				}
			}
			else if (function2 == 2) {
				cout << "���������֤�ţ�" << endl;
				char id[20];
				cin >> id;
				for (int i = 0; i < 20; i++) {
					if (strcmp(id, p[i].id) == 0) {
						cout << "--------------------" << endl;
						cout << "������" << p[i].name << "     ���֤�ţ�" << p[i].id << "     ��ʼվ��" << p[i].f << "     �յ�վ��" << p[i].l << "     Ʊ�ۣ�" << p[i].pay << endl;
						cout << "--------------------" << endl;
						break;
					}
					if (i == 19)
						cout << "û�в�ѯ����" << endl;     //���������û�з��ּ�¼
				}
			}
		}
		cout << "�Ƿ���ʾ��ʱ��λ�����" << endl;
		int kkkk;
		cin >> kkkk;
		if (kkkk == 1) {
			cout << "-----------------" << endl;
			for (int i = 0; i < 10; i++)
				cout << A[i] << "   " << endl;
			cout << "-----------------" << endl;
		}
		/*else if (function == 3) {

			}*/
	}
}





