#include<iostream>
#include<string>
#include<cstdlib>
#include<math.h>
#include<iomanip>
#include<algorithm>
#include<thread>
#include<atomic>
#include<mutex>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
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
void initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "初始化套接字库失败！" << endl;
	}
	else {
		cout << "初始化套接字库成功！" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	else {
		cout << "套接字库版本正确！" << endl;
	}
	//填充服务端地址信息
}
int find_min(int f, int l) {            //查找站点中座位数最小的值，在从f站到l站只有min张票
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
		A[f1 - 1]--;
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
	if (f < 1 || l>10 || f > 9 || l < 2) {
		return -1;
	}
	int f1 = f, l1 = l;
	int seat = find_min(f, l);                //记录座位数量
	return seat;
}
int need_pay(int f, int l) {
	int sum = 0;
	for (int i = f; i < l; i++)
		sum += d[i - 1];
	return sum;
}
int main() {
	for (int i = 0; i < 10; i++) {
		A[i] = 10;                            //初始化座位数量
	}
	for (int i = 0; i < 9; i++) {
		d[i] = (i * i + 32) % 12 + 1;               //生成站点间距离  即价格
	}
	memset(p, 0, sizeof(p));
	//定义长度变量
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//定义发送缓冲区和接受缓冲区
	char send_buf[100];
	char recv_buf[100];
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	SOCKET s_accept;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//填充服务端信息
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(9999);
	//创建套接字
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "套接字绑定失败！" << endl;
		WSACleanup();
	}
	else {
		cout << "套接字绑定成功！" << endl;
	}
	//设置套接字为监听状态
	if (listen(s_server, SOMAXCONN) < 0) {
		cout << "设置监听状态失败！" << endl;
		WSACleanup();
	}
	else {
		cout << "设置监听状态成功！" << endl;
	}
	cout << "服务端正在监听连接，请稍候...." << endl;
	//接受连接请求
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR) {
		cout << "连接失败！" << endl;
		WSACleanup();
		return 0;
	}
	cout << "连接建立，准备接受数据" << endl;
	//接收数据
	while (1) {
		recv_len = recv(s_accept, recv_buf, 100, 0);  //接收功能号
		if (recv_len < 0) {
			break;
		}
		if (recv_buf[0] == '1') {
			strcpy_s(send_buf, "请输入起始站点和终点站(以起始站点/终点站的方式):");
			send_len = send(s_accept, send_buf, 100, 0);
			if (send_len < 0) {
				break;
			}
			recv_len = recv(s_accept, recv_buf, 100, 0);  //接收起点于终点
			if (recv_len < 0) {
				break;
			}

			int f = recv_buf[0] - '0';
			int l = recv_buf[2] - '0'; //获取起点和终点站
			if (recv_buf[1] != '/')
				f = 10;
			if (recv_buf[3] > '0')
				l = 11;
			if (recv_buf[0] == recv_buf[2])
				f = l = 11;                               //这段代码处理非法输入的情况

			int flag = check(f, l);
			string str=to_string(flag);
			string  dd = "有";
			dd += str;
			dd += "个空余座位，价钱为";
			str = to_string(need_pay(f, l));
			dd += str;
			dd += "元。";
			dd += "购票输入1,不购票输入0";
			if (flag > 0) {
				strcpy_s(send_buf, dd.c_str());
				send_len = send(s_accept, send_buf, 100, 0);
			}
			else if (flag == -1) {
				strcpy_s(send_buf, "1输入站点非法！");
				send_len = send(s_accept, send_buf, 100, 0);
			}
			else {
				strcpy_s(send_buf, "2抱歉，没有空余的座位了！");
				send_len = send(s_accept, send_buf, 100, 0);
			}
			recv_len = recv(s_accept, recv_buf, 100, 0);    //接收购票与否信息
			if (recv_len < 0) {
				break;
			}
			if (recv_buf[0] == '1') {
				strcpy_s(send_buf, "请输入姓名：");
				send_len = send(s_accept, send_buf, 100, 0);
				recv_len = recv(s_accept, recv_buf, 100, 0);
				if (recv_len < 0) {
					break;
				}
				strcpy_s(p[num].name, recv_buf);
				strcpy_s(send_buf, "请输入身份证号：");
				send_len = send(s_accept, send_buf, 100, 0);
				recv_len = recv(s_accept, recv_buf, 100, 0);
				if (recv_len < 0) {
					break;
				}
				strcpy_s(p[num].id, recv_buf);
				p[num].f = f;
				p[num].l = l;
				p[num].pay = need_pay(f, l);
				buy_ticket(f, l);      //调用buy_ticket函数购票
				num++;                 //买票人数+1
			}
			else if (recv_buf[0] == '0') {
				continue;
			}
		}
	}
	//关闭套接字
	closesocket(s_server);
	closesocket(s_accept);
	//释放DLL资源
	WSACleanup();
	return 0;
}


