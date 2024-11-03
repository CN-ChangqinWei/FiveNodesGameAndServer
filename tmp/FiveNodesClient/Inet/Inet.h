#pragma once
#include<Winsock2.h>
#include<iostream>
#include<map>
#include"Inet_config.h"
#include<string.h>
#include"QObject"
#include<QThread>

using namespace std;

class CInet:public QObject{
    Q_OBJECT
public:
	CInet(){}
    virtual ~CInet(){}
	virtual bool init()=0;
	virtual void close()=0;
    virtual int send(char* data, int len, unsigned int aimsock)=0;
    virtual int recv(char* buf, int len, unsigned int from)=0;

};

