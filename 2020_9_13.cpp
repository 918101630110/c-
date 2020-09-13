#include<iostream>
#include<string>
#include<math.h>
#include<iomanip>
#include<algorithm>
#include<thread>
#include<atomic>
#include<mutex>
using namespace std;
int A[10];      //A[i]记录第i+1个站时的空座位数
int d[9];    //d[i]记录i+1到i+2站的距离
int num = 0;                 //num变量记录当前买了票的人数
mutex mutx;
struct Person {
	char name[10];   //姓名
	char id[20];	//身份证号
	int f;			//起始站点
	int l;			//目的站点
	int pay;		//需要付的钱
};
Person p[20];
int find_min(int f,int l) {            //查找站点中座位数最小的值，在从f站到l站只有min张票
	int min;
	min = A[f - 1];
	for (int i = f - 1; i < l - 1; i++) {
		if (min > A[i])
			min = A[i];
	}
	return min;
}
void buy_ticket(int f, int l) {  //买票后，从起始站点开始，后面每个站点座位-1直到目的站点，目的站点座位数不减（要下车）
	int f1 = f, l1 = l;
	while (f1 != l1) {
		A[f1-1]--;
		f1++;
	}
}
void refund_ticket(int i) {   ///退票
	num--;                    //买票人数-1
	int f1 = p[i].f;
	int l1 = p[i].l;
	while (f1 != l1) {
		A[f1 - 1]++;
		f1++;
	}
	while (p[i].f != 0) {     //队列往前移动
		p[i] = p[i + 1];
		i++;
	}
}
int check(int f, int l) {
	if (f < 1 || l>10||f>9||l<2) {
		return -1;
	}
	int f1 = f, l1 = l;
	int seat = find_min(f, l);                //记录座位数量
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
		cout << endl<<endl<<"请选择功能：" << endl << "1.查询余票/购票     2.查询车票      3.退票" << endl;
		cin >> function;
		if (function == 1) {
			int f, l;
			cout << "请输入起始站点：";
			cin >> f;
			cout << "请输入到达站点：";
			cin >> l;
			int flag = check(f, l);
			if (flag > 0) {
				cout << "有" << flag << "个空余座位，价钱为" << need_pay(f, l) << "元。" << endl;
			}
			else if (flag == -1) {
				cout << "输入站点非法！" << endl;
				cout << "-----------------------" << endl;
				cout << "线程" << th << "执行完毕" << endl;
				cout << "-----------------------" << endl;
				return;
			}
			else {
				cout << "抱歉，没有空余的座位了！";
				cout << "-----------------------" << endl;
				cout << "线程" << th << "执行完毕" << endl;
				cout << "-----------------------" << endl;
				return;
			}
			cout << "选择购票请输入1,不购票请输入0返回" << endl;
			cin >> flag;
			if (flag) {
				cout << "请输入姓名：";
				cin >> p[num].name;				//num在进入大循环前被置为0
				cout << "请输入身份证号：";
				cin >> p[num].id;
				p[num].f = f;
				p[num].l = l;
				p[num].pay = need_pay(f, l);
				buy_ticket(f, l);      //调用buy_ticket函数购票
				num++;                 //买票人数+1
			}
			else {
				cout << "-----------------------" << endl;
				cout << "线程" << th << "执行完毕" << endl;
				cout << "-----------------------" << endl;
				return;
			}
				
		}
		else if (function == 2) {
			int function2;
			cout << "请选择查询车票的方式：" << endl << "1.姓名    2.身份证号" << endl;
			cin >> function2;
			if (function2 == 1) {
				cout << "请输入姓名：" << endl;
				char name[20];
				cin >> name;
				for (int i = 0; i < 20; i++) {
					if (strcmp(name, p[i].name) == 0) {
						cout << "--------------------" << endl;
						cout << "姓名：" << p[i].name << "     身份证号：" << p[i].id << "     起始站：" << p[i].f << "     终点站：" << p[i].l << "     票价：" << p[i].pay << endl;
						cout << "--------------------" << endl;
						break;
					}
					else if (i == 19)
						cout << "没有查询到！" << endl;     //遍历数组后没有发现记录
				}
			}
			else if (function2 == 2) {
				cout << "请输入身份证号：" << endl;
				char id[20];
				cin >> id;
				for (int i = 0; i < 20; i++) {
					if (strcmp(id, p[i].id) == 0) {
						cout << "查询到结果如下：" << endl;
						cout << "--------------------" << endl;
						cout << "姓名：" << p[i].name << "     身份证号：" << p[i].id << "     起始站：" << p[i].f << "     终点站：" << p[i].l << "     票价：" << p[i].pay << endl;
						cout << "--------------------" << endl;
						break;
					}
					else if (i == 19)
						cout << "没有查询到！" << endl;     //遍历数组后没有发现记录
				}
			}
		}
		else if (function == 3) {
			int function3;
			cout << "请选择退票的方式：" << endl << "1.姓名    2.身份证号" << endl;
			cin >> function3;

			if (function3 == 1) {
				cout << "请输入姓名：" << endl;
				char name[20];
				cin >> name;
				for (int i = 0; i < 20; i++) {
					if (strcmp(name, p[i].name) == 0) {
						refund_ticket(i);
						cout << "退票成功" << "num====" << num << endl;                 //退票
						break;
					}
					else if (i == 19)
						cout << "未查询到该姓名下的火车票！" << endl;
				}
			}
			else if (function3 == 2) {
				cout << "请输入身份证号：" << endl;
				char id[20];
				cin >> id;
				for (int i = 0; i < 20; i++) {
					if (strcmp(id, p[i].id) == 0) {
						refund_ticket(i);
						cout << "退票成功" << "num====" << num << endl;
						break;
					}
					//退票
					else if (i == 19)
						cout << "未查询到该身份证号下的火车票！" << endl;
				}
			}

		}
		cout << "-----------------------" << endl;
		cout << "线程" << th << "执行完毕" << endl;
		cout << "-----------------------" << endl;
		/*cout << "是否显示此时座位情况？" << endl;
		int kkkk;
		cin >> kkkk;
		if (kkkk == 1) {
			cout << "-----------------" << endl;
			for (int i = 0; i < 10; i++) {
				cout << i + 1 << "号站：";
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
		A[i] = 10;                            //初始化座位数量
	}
	for (int i = 0; i < 9; i++) {
		d[i] = (i*i+32) % 12+1;               //生成站点间距离  即价格
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 30; j++) {
			if (i == 2 && j == 6) {
				cout << " 火车模拟购票系统 ";
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





