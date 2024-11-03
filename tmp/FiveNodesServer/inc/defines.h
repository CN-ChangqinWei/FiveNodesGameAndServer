#ifndef _DEFINES_H
#define _DEFINES_H

#include<iostream>
using namespace std;

#ifdef _DEBUG_FUNC
#define _FUNC_SHOW(x)	(cout<<__func__<<x<<endl)
#else
#define _FUNC_SHOW(x)	(x)
#endif

#define _FUNC_EXCUTE(func) \
	DealMsg* dealmsg=(DealMsg*)arg;\
	CEpollServer::server->deal_##func(dealmsg->data,dealmsg->m_sFrom);\
	delete []dealmsg->data;\
	delete dealmsg;


#endif
