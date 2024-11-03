#pragma once
#include"Inet.h"

class CTCPServer :public CInet {
public:
	CTCPServer();
	~CTCPServer();
	bool init();
	void close();
    int send(char* data, int len, unsigned int aimsock);
    int recv(char* buf, int len, unsigned int from);
	SOCKET m_socklisten,m_sock;
	int run();
};
