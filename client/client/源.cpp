#include<iostream>
#include<string>
#include<math.h>
#include<cstdlib>
#include<iomanip>
#include<algorithm>
#include<thread>
#include<atomic>
#include<mutex>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
mutex mutx;
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
int main() {
	//���峤�ȱ���
	int send_len = 0;
	int recv_len = 0;
	//���巢�ͻ������ͽ��ܻ�����
	char send_buf[100];
	char recv_buf[100];
	//���������׽��֣����������׽���
	SOCKET s_server;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9999);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "����������ʧ�ܣ�" << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "���������ӳɹ���" << endl;
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
	while (1) {
		cout << endl << endl << "��ѡ���ܣ�" << endl << "1.��ѯ��Ʊ/��Ʊ     2.��ѯ��Ʊ      3.��Ʊ" << endl;

		cin >> send_buf;
		char func1 = send_buf[0];   //func1��¼����
		if (func1 == '1') {
			send_len = send(s_server, send_buf, 100, 0);
			if (send_len < 0) {
				break;
			}
			recv_len = recv(s_server, recv_buf, 100, 0); 
			if (recv_len < 0) {
				break;
			}
			else {
				cout << recv_buf << endl;   //����������յ�
			}
			cin >> send_buf;             //�������վ���յ�վ
			send_len = send(s_server, send_buf, 100, 0);
			if (send_len < 0) {
				break;
			}
			recv_len = recv(s_server, recv_buf, 100, 0);
			if (recv_len < 0) {
				break;
			}
			else {
				cout << recv_buf << endl;  //Ʊ����Ϣ
			}
			if (recv_buf[0] == '1' || recv_buf[0] == '2') {
				send_buf[0] = '0';
				send_len = send(s_server, send_buf, 100, 0);//�޷�������Ʊ �൱��ֱ������
				continue;
			}
			cin>>send_buf;         //����1��0ѡ��Ʊ���
			if (send_buf[0] == '0') {
				cout << "��ӭ�´�ʹ�ã�" << endl;
				send_len = send(s_server, send_buf, 100, 0);
				continue;
			}//����Ʊ��ֱ���˳�
			send_len = send(s_server, send_buf, 100, 0);
			if (send_len < 0) {
				break;
			}
			recv_len = recv(s_server, recv_buf, 100, 0);
			if (recv_len < 0) {
				break;
			}
			else {
				cout << recv_buf << endl;  //����������
			}
			cin >> send_buf;  //��������
			send_len = send(s_server, send_buf, 100, 0);
			if (send_len < 0) {
				break;
			}
			recv_len = recv(s_server, recv_buf, 100, 0);
			if (recv_len < 0) {
				break;
			}
			else {
				cout << recv_buf << endl; //���������֤��
			}
			cin >> send_buf;//�������֤��
			send_len = send(s_server, send_buf, 100, 0);
		}
		else if (func1 == '2') {

		}
		else if (func1 == '3') {

		}
		else
			cout << "��Ч�Ĺ��ܱ�ţ�" << endl;
	}
	//�ر��׽���
	closesocket(s_server);
	//�ͷ�DLL��Դ
	WSACleanup();
	return 0;
}