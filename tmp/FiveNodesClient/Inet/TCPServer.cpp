#include"TCPServer.h"
CTCPServer::CTCPServer() :m_sock(INVALID_SOCKET) {

	init();

}
CTCPServer::~CTCPServer() {
	close();
}
bool CTCPServer::init() {
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(version, &data)) {
		cout << __func__ << "初始化库失败！" << WSAGetLastError() << endl;
		return -1;
	}
	//if (2 != HIWORD(data.wVersion) || 2 != LOWORD(data.wVersion)) {
	//	cout << __func__ << "库版本错误!" << endl;
	//	close();
	//	return -1;
	//}
	if (INVALID_SOCKET == (m_socklisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
		cout << __func__ << "套接字错误！" << WSAGetLastError() << endl;
		close();
		return -1;
	}
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(_DEF_PORT);
	addr.sin_family = AF_INET;

	if(bind(m_socklisten, (sockaddr*)&addr, sizeof(addr))){
		cout << __func__ << "绑定网卡错误" << WSAGetLastError()<<endl;
		return -1;
	}

	if (listen(m_socklisten, 200)) {
		cout << __func__ << "监听失败！" << WSAGetLastError() << endl;
		close();
		return -1;
	}
	cout << "TCPServer初始化成功" << endl;
	return 0;
}
void CTCPServer::close() {

	if (INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
		cout << "关闭套接字" << endl;
	}
	WSACleanup();
	cout << "卸载库" << endl;
	m_sock = INVALID_SOCKET;

}
int CTCPServer::send(char* data, int len, unsigned int aimsock) {

	if (len <= 0) {
		cout << __func__ << "发送包长度错误" << endl;
		return -1;
	}
	if (data == nullptr) {
		cout << __func__ << "发送数据为空指针" << endl;
		return -1;
	}
	if (::send(m_sock, (char*)&len, sizeof(len), 0) <= 0) {
		cout << __func__ << "发送包长度失败" << WSAGetLastError() << endl;
		return -1;

	}
	int num;
	if ((num = ::send(m_sock, data, len, 0)) <= 0) {
		cout << __func__ << "发送数据失败" << WSAGetLastError() << endl;
		return -1;
	}
	return num;
}
int CTCPServer::recv(char* buf, int len, unsigned int from) {


	if (::recv(m_sock, (char*)&len, sizeof(int), 0) <= 0) {
		cout << __func__ << "接收包长度错误" << WSAGetLastError() << endl;

		return -1;
	}

	int num;
	if ((num = ::recv(m_sock, buf, len, 0)) <= 0) {
		cout << __func__ << "接收数据错误" << WSAGetLastError() << endl;

		return -1;
	}


	return num;
}

int CTCPServer::run() {
	//int errcode = 0;
	sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	if (INVALID_SOCKET==(m_sock = accept(m_socklisten, (sockaddr*)&clientaddr, &addrlen))) {
		cout << __func__ << "接收失败" << WSAGetLastError() << endl;
		return -1;
	}
	cout << inet_ntoa(clientaddr.sin_addr) << "连接" << endl;
	char buf[1024] = {0};
	while (recv(buf, sizeof(buf), 0)>0) {
		
		cout << buf<<endl;
		if (send(buf, strlen(buf), 0) <= 0) {
			return -1;
		}
		memset(buf, 0, sizeof(buf));
	}

	return 0;
}
