#include<iostream>
#include<string>
#include<math.h>
#include<iomanip>
#include<algorithm>
using namespace std;
int A[10];      //A[i]记录第i+1个站时的空座位数
int d[9];    //d[i]记录i+1到i+2站的距离
int num = 0;                 //num变量记录当前买了票的人数
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
				cout << "有"<<flag<<"个空余座位，价钱为" << need_pay(f, l) << "元。" << endl;
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
				buy_ticket(f, l);      //调用buy_ticket函数购票
				num++;                 //买票人数+1
			}
			else
				continue;
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
						cout<<"退票成功" << "num====" << num <<endl;                 //退票
					}
				}
			}
			else if (function3 == 2) {
				cout << "请输入身份证号：" << endl;
				char id[20];
				cin >> id;
				for (int i = 0; i < 20; i++) {
					if (strcmp(id, p[i].id) == 0) {
						refund_ticket(i);
						cout << "退票成功" <<"num===="<<num<< endl;
					}                                              //退票
				}
			}

		}
		cout << "是否显示此时座位情况？" << endl;
		int kkkk;
		cin >> kkkk;
		if (kkkk == 1) {
			cout << "-----------------" << endl;
			for (int i = 0; i < 10; i++) {
				cout << i + 1 << "号站：";
				cout << A[i] << "   " << endl;
			}
			cout << "-----------------" << endl;
		}
		/*else if (function == 3) {

			}*/
	}
}





