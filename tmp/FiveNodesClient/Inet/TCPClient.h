#pragma once
#include"Inet.h"
class CTCPMediator;
class CTCPClient :public CInet {
public:
    CTCPClient(CTCPMediator* _med);
	~CTCPClient();
	bool init();
	void close();
    int send(char* data, int len, unsigned int aimsock);
    int recv(char* buf, int len, unsigned int from);
	int run();
    void recvData();
    static unsigned __stdcall RecvThread(void* pThis);

public slots:
    void slot_send(char* data, int len, unsigned int aimsock);
    void slot_recv(char* buf, int len, unsigned int from);
    void slot_start();
private:
    CTCPMediator* m_pMed;
    QThread* m_pThread;
    bool m_fStart;
    HANDLE m_hRecv;
    SOCKET m_sock;
};
