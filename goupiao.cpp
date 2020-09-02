#include<iostream>
#include<string>
#include<math.h>
#include<iomanip>
#include<algorithm>
using namespace std;
int A[10];      //A[i]记录第i个站时得空座位数
int d[9];    //d[i]记录i+1到i+2站得距离
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
			flag = 0;  //发现有站点没空座位
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
				cout << " 火车模拟购票系统 ";
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
		cout << "请选择功能：" << endl << "1.查询余票/购票     2.查询车票      3.退票" << endl;
		cin >> function;
		if (function == 1) {
			int f, l;
			cout << "请输入起始站点：";
			cin >> f;
			cout << "请输入到达站点：";
			cin >> l;
			int flag = check(f, l);
			if (flag) {
				cout << "有空余座位，价钱为" << need_pay(f, l) << "元。" << endl;
			}
			else {
				cout << "抱歉，没有空余的座位了！";
				continue;
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
				buy_ticket(f, l);      //调用buy函数购票
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
			cout << "请选择查询车票的方式：" << endl << "1.姓名    2.身份证号" << endl;
			cin >> function2;
			if (function2 == 1) {
				cout << "请输入姓名：" << endl;
				char name[20];
				cin >> name;
				for (int i = 0; i < 20; i++) {
					if (strcmp(name,p[i].name)==0) {
						cout << "--------------------" << endl;
						cout << "姓名：" << p[i].name << "     身份证号：" << p[i].id << "     起始站：" << p[i].f << "     终点站：" << p[i].l << "     票价：" << p[i].pay << endl;
						cout << "--------------------" << endl;
						break;
					}
					if (i == 19)
						cout << "没有查询到！" << endl;     //遍历数组后没有发现记录
				}
			}
			else if (function2 == 2) {
				cout << "请输入身份证号：" << endl;
				char id[20];
				cin >> id;
				for (int i = 0; i < 20; i++) {
					if (strcmp(id, p[i].id) == 0) {
						cout << "--------------------" << endl;
						cout << "姓名：" << p[i].name << "     身份证号：" << p[i].id << "     起始站：" << p[i].f << "     终点站：" << p[i].l << "     票价：" << p[i].pay << endl;
						cout << "--------------------" << endl;
						break;
					}
					if (i == 19)
						cout << "没有查询到！" << endl;     //遍历数组后没有发现记录
				}
			}
		}
		cout << "是否显示此时座位情况？" << endl;
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





