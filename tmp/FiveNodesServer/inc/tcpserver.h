#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include"inet.h"
#include"config.h"
#define _DEF_IP		(INADDR_ANY)

class Ctcpserver:public CInet{
public:
	Ctcpserver();
	~Ctcpserver();
	int init();
	void close();
	int send(char* data,int len,int sockto);
	int recv(char* buf,int len,int sockfrm);
	int acp(struct sockaddr* addr,socklen_t size); 		
	int getsock();
private:
	int m_sock;

};




#endif
