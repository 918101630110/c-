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
void initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
	}
	else {
		cout << "��ʼ���׽��ֿ�ɹ���" << endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "�׽��ֿ�汾�Ų�����" << endl;
		WSACleanup();
	}
	else {
		cout << "�׽��ֿ�汾��ȷ��" << endl;
	}
	//������˵�ַ��Ϣ
}
int find_min(int f, int l) {            //����վ������λ����С��ֵ���ڴ�fվ��lվֻ��min��Ʊ
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
		A[f1 - 1]--;
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
	if (f < 1 || l>10 || f > 9 || l < 2) {
		return -1;
	}
	int f1 = f, l1 = l;
	int seat = find_min(f, l);                //��¼��λ����
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
		A[i] = 10;                            //��ʼ����λ����
	}
	for (int i = 0; i < 9; i++) {
		d[i] = (i * i + 32) % 12 + 1;               //����վ������  ���۸�
	}
	memset(p, 0, sizeof(p));
	//���峤�ȱ���
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//���巢�ͻ������ͽ��ܻ�����
	char send_buf[100];
	char recv_buf[100];
	//���������׽��֣����������׽���
	SOCKET s_server;
	SOCKET s_accept;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(9999);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "�׽��ְ�ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else {
		cout << "�׽��ְ󶨳ɹ���" << endl;
	}
	//�����׽���Ϊ����״̬
	if (listen(s_server, SOMAXCONN) < 0) {
		cout << "���ü���״̬ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else {
		cout << "���ü���״̬�ɹ���" << endl;
	}
	cout << "��������ڼ������ӣ����Ժ�...." << endl;
	//������������
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR) {
		cout << "����ʧ�ܣ�" << endl;
		WSACleanup();
		return 0;
	}
	cout << "���ӽ�����׼����������" << endl;
	//��������
	while (1) {
		recv_len = recv(s_accept, recv_buf, 100, 0);  //���չ��ܺ�
		if (recv_len < 0) {
			break;
		}
		if (recv_buf[0] == '1') {
			strcpy_s(send_buf, "��������ʼվ����յ�վ(����ʼվ��/�յ�վ�ķ�ʽ):");
			send_len = send(s_accept, send_buf, 100, 0);
			if (send_len < 0) {
				break;
			}
			recv_len = recv(s_accept, recv_buf, 100, 0);  //����������յ�
			if (recv_len < 0) {
				break;
			}

			int f = recv_buf[0] - '0';
			int l = recv_buf[2] - '0'; //��ȡ�����յ�վ
			if (recv_buf[1] != '/')
				f = 10;
			if (recv_buf[3] > '0')
				l = 11;
			if (recv_buf[0] == recv_buf[2])
				f = l = 11;                               //��δ��봦��Ƿ���������

			int flag = check(f, l);
			string str=to_string(flag);
			string  dd = "��";
			dd += str;
			dd += "��������λ����ǮΪ";
			str = to_string(need_pay(f, l));
			dd += str;
			dd += "Ԫ��";
			dd += "��Ʊ����1,����Ʊ����0";
			if (flag > 0) {
				strcpy_s(send_buf, dd.c_str());
				send_len = send(s_accept, send_buf, 100, 0);
			}
			else if (flag == -1) {
				strcpy_s(send_buf, "1����վ��Ƿ���");
				send_len = send(s_accept, send_buf, 100, 0);
			}
			else {
				strcpy_s(send_buf, "2��Ǹ��û�п������λ�ˣ�");
				send_len = send(s_accept, send_buf, 100, 0);
			}
			recv_len = recv(s_accept, recv_buf, 100, 0);    //���չ�Ʊ�����Ϣ
			if (recv_len < 0) {
				break;
			}
			if (recv_buf[0] == '1') {
				strcpy_s(send_buf, "������������");
				send_len = send(s_accept, send_buf, 100, 0);
				recv_len = recv(s_accept, recv_buf, 100, 0);
				if (recv_len < 0) {
					break;
				}
				strcpy_s(p[num].name, recv_buf);
				strcpy_s(send_buf, "���������֤�ţ�");
				send_len = send(s_accept, send_buf, 100, 0);
				recv_len = recv(s_accept, recv_buf, 100, 0);
				if (recv_len < 0) {
					break;
				}
				strcpy_s(p[num].id, recv_buf);
				p[num].f = f;
				p[num].l = l;
				p[num].pay = need_pay(f, l);
				buy_ticket(f, l);      //����buy_ticket������Ʊ
				num++;                 //��Ʊ����+1
			}
			else if (recv_buf[0] == '0') {
				continue;
			}
		}
	}
	//�ر��׽���
	closesocket(s_server);
	closesocket(s_accept);
	//�ͷ�DLL��Դ
	WSACleanup();
	return 0;
}


