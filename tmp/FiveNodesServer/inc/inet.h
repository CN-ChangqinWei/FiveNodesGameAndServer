#ifndef _INET_H
#define _INET_H
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include<strings.h>
#include<stdio.h>
class CInet{
public:
	CInet(){}
	virtual ~CInet(){}
	virtual int init()=0;
	virtual void close()=0;
	virtual int send(char*data,int len,int sockto)=0;
	virtual int recv(char*buf,int len,int sockfrm)=0;
};



#endif
