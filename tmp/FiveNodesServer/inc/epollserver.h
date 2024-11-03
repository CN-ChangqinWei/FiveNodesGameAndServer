#ifndef _EPOLLSERVER_H
#define _EPOLLSERVER_H
#include<iostream>
#include"type.h"
#include"threadspoll.h"
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<unistd.h>
#include"tcpserver.h"
#include<map>
#include<list>
#include"protocl.h"
#include"servermap.h"
#include"defines.h"
#include<mysql/mysql.h>
#include"CMySql.h"
#include<vector>
#include"../inc/zone.h"
#include<mutex>
#define _DEF_EPOLL_MAX	(200)
#define _DEF_SERVER	(CEpollServer::server)
using namespace std;
class CEpollServer;
typedef void(CEpollServer::*EPOLL_FUNC)(char* buf,int len,int sock);

class CEpollServer{
public:
	static CEpollServer* server;
	static CEpollServer* CreateServer();
	static bool isServerExist();
	static void closeServer();
	typedef void* (*FUNC)(void*);
	CEpollServer();
	~CEpollServer();
	void init();
	bool mysqlinit();
	void close();
	int send(char* data,int len,int aim);
	int recv(char* buf,int len,int frm);
	int deal(char* data,int len,int frm);
	int acp();	
	void run();	
	bool isRun();
	void mapinit();
	void seallst(int fd);
	void addlst(int fd);
	void addaddr(int fd,sockaddr_in);
	void deladdr(int fd);
//	player info

	int get_playericon(int id);
	UserInfo* get_playerinfo(int id);	
int get_playersock(int id);
		
//	接收业务
	void deal_login(char* data,int from);
	void deal_register(char* data,int from);
	void deal_joinzone(char* data,int from);
	void deal_leavezone(char* data,int from);
	void deal_createroom(char* data,int from);
	void deal_joinroom(char* data,int from);
	void deal_leaveroom(char* data,int from);
	void deal_refreshrooms(char* data,int from);
	void deal_piecedown(char* data,int from);
	void deal_win(char* data,int from);
	void deal_winact(char* data,int from);
	void deal_startgame(char* data,int from);
//	发送业务
	void send_userinfo(int userid,int aim);
	void send_friends(int userid,int aim);
	void send_gameroominfo(int zoneid,int roomid);	
private:
	int sock;
	int m_epfd;
	epoll_event m_node;
	epoll_event m_rdlist[_DEF_EPOLL_MAX];
	Ctcpserver* ser;	
	bool m_isrun;
	map<int,FUNC> m_mpfuncs;
	//CThreadspoll* msgpoll;
	map<int,sockaddr_in> m_mpaddrs;
	map<int,int>	m_mpIdtoSock;
	map<int,int>	m_mpSocktoId;
	vector<CZone*> m_veczones;	
	CThreadspoll* dealpoll;
	pthread_t epthread;
	CMySql* m_pSql;
	mutex m_lock;
};
#endif
