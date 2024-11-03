#include"TCPClient.h"
#include"../Mediator/TCPMediator.h"
CTCPClient::CTCPClient(CTCPMediator* _med):m_sock(INVALID_SOCKET),m_fStart(true),m_hRecv(nullptr){
    m_pMed=_med;
    m_pThread=new QThread;
    //moveToThread(m_pThread);
	init();

}
CTCPClient::~CTCPClient() {
	close();
}
unsigned __stdcall CTCPClient:: RecvThread(void* pThis){
    CTCPClient* pClient=(CTCPClient*)pThis;
    pClient->recvData();
    return 0;

}

bool CTCPClient::init() {
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(version, &data)) {
		cout<< __func__ <<"初始化库失败！"<< WSAGetLastError() << endl;
		return -1;
	}
	//if (2 != HIWORD(data.wVersion) || 2 != LOWORD(data.wVersion)) {
	//	cout << __func__ << "库版本错误!" << endl;
	//	close();
	//	return -1;
	//}
	if (INVALID_SOCKET==(m_sock=socket(AF_INET,SOCK_STREAM, IPPROTO_TCP))) {
		cout << __func__ << "套接字错误！" << WSAGetLastError() << endl;
		close();
		return -1;
	}
	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr(_DEF_SERVERIP);
	addr.sin_port = htons(_DEF_PORT);
    addr.sin_family = AF_INET;
    if (::connect(m_sock, (sockaddr*)&addr, sizeof(addr))) {
		cout << __func__ << "连接失败！" << WSAGetLastError() << endl;
		close();
		return -1;
	}
    m_hRecv=(HANDLE)_beginthreadex(NULL,0,&RecvThread,this,0,NULL);
	cout << "TCPClient初始化成功" << endl;
	return 0;
}
void CTCPClient::close() {
    m_fStart=false;
    if(m_hRecv){
        if(WAIT_TIMEOUT==WaitForSingleObject(m_hRecv,1000)){
            TerminateThread(m_hRecv,-1);
        }
        CloseHandle(m_hRecv);
        m_hRecv=nullptr;
    }
	if (INVALID_SOCKET != m_sock) {
		closesocket(m_sock);
		cout << "关闭套接字" << endl;
	}
	WSACleanup();
	cout << "卸载库" << endl;
	m_sock = INVALID_SOCKET;

}
int CTCPClient::send(char* data, int len, unsigned int aimsock) {
    cout<<__func__<<endl;
	if (len <= 0) {
		cout<< __func__<< "发送包长度错误" << endl;
		return -1;
	}
	if (data == nullptr) {
		cout << __func__ << "发送数据为空指针" << endl;
		return -1;
	}
	if (::send(m_sock, (char*)&len, sizeof(len), 0) <= 0) {
		cout << __func__ << "发送包长度失败" <<WSAGetLastError()<< endl;
		return -1;
	
	}
	int num;
	if ((num=::send(m_sock, data, len, 0)) <= 0) {
		cout << __func__ << "发送数据失败" <<WSAGetLastError()<<endl;
		return -1;
	}
    delete []data;
	return num;
}
int CTCPClient::recv(char* buf, int len, unsigned int from) {
    cout<<__func__<<endl;
    if(nullptr==buf){
        cout<<__func__<<"buf is null"<<endl;
        return -1;
    }
	if (::recv(m_sock, (char*)&len, sizeof(int), 0)<=0) {
		cout << __func__ << "接收包长度错误" << WSAGetLastError() << endl;
		
		return -1;
	}

	int num;
	if ((num=::recv(m_sock, buf, len, 0))<=0) {
		cout << __func__ << "接收数据错误" << WSAGetLastError() << endl;
		
		return -1;
	}


	return num;
}

int CTCPClient::run() {
	char buf[1024] = { 0 };
	while (cin >> buf) {
		
		if (send(buf, strlen(buf), 0) <= 0) {
			return -1;
		}
		memset(buf, 0, sizeof(buf));
		if (recv(buf, sizeof(buf), 0)<=0) {
			return -1;
		}
		cout << buf << endl;
		memset(buf, 0, sizeof(buf));
		cout << "runing" << endl;
	}
	return 0;
}

void CTCPClient::slot_send(char* data, int len, unsigned int aimsock){
    send(data,len,aimsock);
}
void CTCPClient::slot_recv(char* buf, int len, unsigned int from){
    if(-1==recv(buf,len,from)) return;
    m_pMed->deal(buf,len,from);
}

void CTCPClient::slot_start(){
     if(m_fStart) return;
     m_fStart=true;
     char* buf=nullptr;
     while(m_fStart){
         buf=new char[4096];
         memset(buf,0,4096);
         recv(buf,4096,m_sock);
         m_pMed->deal(buf,4096,m_sock);
     }
}

void CTCPClient::recvData(){
    char* buf;
    while(m_fStart){
        buf=(char*)malloc(4096);
        if(-1==recv(buf,4096,0)){
            m_fStart=0;
            cout<<"server err"<<endl;
        }
        m_pMed->deal(buf,4096,m_sock);

    }


}
