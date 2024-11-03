#include"../inc/epollserver.h"
CEpollServer* CEpollServer::server=new CEpollServer;

CEpollServer::CEpollServer():m_isrun(true){

	m_veczones=vector<CZone*>(_DEF_NOF_GAMES,nullptr);	
	for(int i=0;i<_DEF_NOF_GAMES;i++){
		m_veczones[i]=new CZone(i,_DEF_ZONE_MAXROOMS);	
	}

}

CEpollServer::~CEpollServer(){
	delete dealpoll;
	delete m_pSql;
	delete ser;
}



void *EpollRun(void* arg){
	pthread_detach(pthread_self());
	_FUNC_SHOW(0);
	if(nullptr==arg) return nullptr;
	CEpollServer* epoll=(CEpollServer*)arg;
	while(epoll->isRun()){		
		epoll->run();
	}

	return nullptr;
}



CEpollServer* CEpollServer::CreateServer()	{
	_FUNC_SHOW(0);
	if(CEpollServer::server){
		return CEpollServer::server;
	}
	CEpollServer::server=new CEpollServer;
	return CEpollServer::server;
}

bool CEpollServer::isServerExist(){
	_FUNC_SHOW(0);
	if(CEpollServer::server) return true;
	return false;
}

void CEpollServer::closeServer(){
	_FUNC_SHOW(0);
	if(CEpollServer::server){
		delete CEpollServer::server;
	}
	CEpollServer::server=nullptr;
}

void CEpollServer::init(){

	_FUNC_SHOW(0);
	if(mysqlinit()){
		cout<<"epoll init fail"<<endl;
		return ;
	}
	mapinit();
	ser=new Ctcpserver;
	sock=ser->getsock();	
	if(-1==sock){
		cout<<"epoll 初始化失败 tcpserver 初始化失败"<<endl;
		return;
	}
	m_epfd=epoll_create(_DEF_EPOLL_MAX);
	m_node.data.fd=sock;
	m_node.events=EPOLLIN;
	epoll_ctl(m_epfd,EPOLL_CTL_ADD,m_node.data.fd,&m_node);		
	//msgpoll=new CThreadspoll(50,5,100);
	dealpoll=new CThreadspoll(50,5,100);
	pthread_create(&epthread,NULL,EpollRun,this);		
}

bool CEpollServer::mysqlinit(){
	m_pSql=new CMySql;

	if(0==m_pSql->ConnectMySql(_DEF_MYSQL_HOST,_DEF_MYSQL_USER,_DEF_MYSQL_PSWD,_DEF_MYSQL_DB)){
		cout<<"Mysql connect fail"<<endl;
		delete m_pSql;
		return -1;
	}
	cout<<"Mysql connect ok"<<endl;
	return 0;

}

void CEpollServer::close(){
	_FUNC_SHOW(0);
	m_isrun=false;
	exit(0);
}
int CEpollServer::send(char* data,int len,int aim){
	_FUNC_SHOW(0);
	if(ser->send(data,len,aim)){
		return -1;
	}	
	return 0;
}
int CEpollServer::recv(char* buf,int len,int frm){
	_FUNC_SHOW(0);

	if(ser->recv(buf,len,frm)){
		int id=m_mpSocktoId[frm];
		m_mpIdtoSock.erase(m_mpIdtoSock.find(id));
		m_mpSocktoId.erase(m_mpSocktoId.find(frm));
		for(int i=0;i<_DEF_NOF_GAMES;i++)
			m_veczones[i]->del_player(id);
		return -1;
	} 
	deal(buf,len,frm);
	return 0;
}
int CEpollServer::deal(char* data,int len,int frm){

	_FUNC_SHOW(0);
	if(nullptr==data) return -1;
	//pthread_t tid;
	int type=*((int*)data)-_DEF_TCPBASE;
	cout<<"msg type "<<type<<endl;
	if(type<0||type>=_DEF_NOF_FUNCS){
		cout<<"deal types error"<<type<<endl;
		delete []data;
		return -1;
	}
	FUNC fun=m_mpfuncs[type];

	if(fun){
	//	lock_guard<mutex> lg(m_lock);
		DealMsg* dealmsg=new DealMsg;
		dealmsg->data=data;
		dealmsg->m_sFrom=frm;	
	//	dealpoll->task_add(fun,dealmsg);
		fun(dealmsg);
//		delete []data;
//		delete dealmsg;
	}
	else{

		cout<<"deal types error"<<type<<endl;
		delete []data;
		return 0;
	}

	return 0;	
}
int CEpollServer::acp(){
	_FUNC_SHOW(0);
	struct sockaddr_in addr;
	int clientsock;
	clientsock=ser->acp((struct sockaddr*)&addr,sizeof(addr));
	if(sock!=-1){
		cout<<inet_ntoa(addr.sin_addr)<<"connected \n";
		m_node.data.fd=clientsock;
	}else {
		perror("accept fail");
		return -1;
	}
	//int flag=fcntl(clientsock,F_GETFL);
	//flag=flag|O_NONBLOCK|O_RDWR;
	//clientsock=fcntl(clientsock,F_SETFL,flag);
	epoll_ctl(m_epfd,EPOLL_CTL_ADD,clientsock,&m_node);
	m_node.data.fd=sock;
	epoll_ctl(m_epfd,EPOLL_CTL_ADD,sock,&m_node);
	return clientsock;
}
void CEpollServer::run(){
	_FUNC_SHOW(0);
	int num;	
	num=epoll_wait(m_epfd,m_rdlist,_DEF_EPOLL_MAX,100);
	int flag=0;
	int fd=0;
	while(num){
		if((fd=m_rdlist[flag].data.fd)==sock){//server
			epoll_ctl(m_epfd,EPOLL_CTL_DEL,fd,NULL);
			dealpoll->task_add(func_accept,(void*)fd);
		}else{//client
			epoll_ctl(m_epfd,EPOLL_CTL_DEL,fd,NULL);
			dealpoll->task_add(func_dealmsg,(void*)fd);
		}
		num--;
		flag++;
	}
}
bool CEpollServer::isRun(){
	_FUNC_SHOW(0);
	return m_isrun;
}

void CEpollServer::mapinit(){
	_FUNC_SHOW(0);
	m_mpfuncs[_DEF_TCP_SHOW-_DEF_TCPBASE]=func_show;
	m_mpfuncs[_DEF_TCP_TIME-_DEF_TCPBASE]=func_time;
	m_mpfuncs[_DEF_TCP_LOGIN-_DEF_TCPBASE]=func_login;
	m_mpfuncs[_DEF_TCP_REGISTER-_DEF_TCPBASE]=func_register;
	m_mpfuncs[_DEF_TCP_JOINZONE-_DEF_TCPBASE]=func_joinzone;
	m_mpfuncs[_DEF_TCP_LEAVEZONE-_DEF_TCPBASE]=func_leavezone;
	m_mpfuncs[_DEF_TCP_CREATEROOM-_DEF_TCPBASE]=func_createroom;
	m_mpfuncs[_DEF_TCP_JOINROOM-_DEF_TCPBASE]=func_joinroom;
	m_mpfuncs[_DEF_TCP_LEAVEROOM-_DEF_TCPBASE]=func_leaveroom;
	m_mpfuncs[_DEF_TCP_REFRESHROOM-_DEF_TCPBASE]=func_refreshrooms;	
	m_mpfuncs[_DEF_TCP_PIECEDOWN-_DEF_TCPBASE]=func_piecedown;
	m_mpfuncs[_DEF_TCP_WIN-_DEF_TCPBASE]=func_win;
	m_mpfuncs[_DEF_TCP_WINACT-_DEF_TCPBASE]=func_winact;
	m_mpfuncs[_DEF_TCP_STARTGAME-_DEF_TCPBASE]=func_startgame;
}

void CEpollServer::seallst(int fd){
	_FUNC_SHOW(0);
	epoll_ctl(m_epfd,EPOLL_CTL_DEL,fd,NULL); 
}

void CEpollServer::addlst(int fd){
	_FUNC_SHOW(0);
	m_node.data.fd=fd;
	epoll_ctl(m_epfd,EPOLL_CTL_ADD,fd,&m_node);
}

int CEpollServer::get_playericon(int id){
	lock_guard<mutex> lg(m_lock);	
	char sqlcmd[100]={0};
	sprintf(sqlcmd,"select icon from t_userinfo where ID = '%d';",id);
	list<string> res;
	m_pSql->SelectMySql(sqlcmd,1,res);
	int icon;
	if(res.empty())		
		return -1;
	else{
		icon=atoi(res.front().c_str());

	}
	return icon;
}

UserInfo* CEpollServer::get_playerinfo(int id){
	lock_guard<mutex> lg(m_lock);	
	char sqlcmd[100]={0};
	sprintf(sqlcmd,"select ID,Name,sign,icon from t_userinfo where ID = '%d';",id);
	list<string> res;
	m_pSql->SelectMySql(sqlcmd,4,res);
	if(res.empty()) return nullptr;
	UserInfo* info=new UserInfo;
	info->id=atoi(res.front().c_str());
	cout<<"ID: "<<info->id<<endl;
	res.pop_front();

	strcpy(info->m_name,res.front().c_str());
	cout<<"Name: "<<info->m_name<<endl;
	res.pop_front();

	strcpy(info->m_sign,res.front().c_str());
	cout<<"sign: "<<info->m_sign<<endl;
	res.pop_front();

	info->icon=atoi(res.front().c_str());
	cout<<"icon: "<<info->icon<<endl;
	res.pop_front();

	return info;
}

int CEpollServer::get_playersock(int id){	
	lock_guard<mutex> lg(m_lock);	
	if(m_mpIdtoSock.find(id)!=m_mpIdtoSock.end()){
		return m_mpIdtoSock[id];
	}
	return -1;
}

void CEpollServer::deal_login(char* data,int from){

	//lock_guard<mutex> lg(m_lock);
	char sqlcmd[100]={0};
	Msg_Login* msg=(Msg_Login*)data;
	sprintf(sqlcmd,"select pswd,Name,ID from t_userinfo where tel = '%s';",msg->m_login.m_tel);
	list<string> res;
	string loginmsg[3];
	m_pSql->SelectMySql(sqlcmd,3,res);
	Reply re;
	re.m_type=_DEF_TCP_LOGIN;
	if(res.empty()){
		re.m_res=_DEF_LOGIN_USERNOTEXIST;
	}else{
		int i=0;
		while(!res.empty()){
			cout<<res.front()<<endl;
			loginmsg[i++]=res.front();
			res.pop_front();
		}	

		//cmp pswd
		if(strcmp(loginmsg[0].c_str(),msg->m_login.m_pswd)){
			re.m_res=_DEF_LOGIN_PSWDERR;
		}else{
			re.m_res=_DEF_LOGIN_OK;
			//strcpy(re.data.m_user.m_name,loginmsg[1].c_str());
			re.data.m_user.ID=atoi(loginmsg[2].c_str());
			if(m_mpIdtoSock.find(re.data.m_user.ID)!=m_mpIdtoSock.end()){
				re.m_res=_DEF_LOGIN_EXIST;
			}else{
				m_mpIdtoSock[re.data.m_user.ID]=from;
				m_mpSocktoId[from]=re.data.m_user.ID;
			}
		}
	}

	send((char*)&re,sizeof(Reply),from);
	send_userinfo(re.data.m_user.ID,from);
}

void CEpollServer::deal_register(char* data,int from){
	cout<<__func__<<endl;
	char sqlcmd[200]={0};
	Msg_Register* msg=(Msg_Register*)data;
	sprintf(sqlcmd,"select ID from t_userinfo where Name = '%s' or tel = '%s';",msg->m_reg.m_name,msg->m_reg.m_tel);
	list<string> res;
	Reply re;
	re.m_type=_DEF_TCP_REGISTER;
	m_pSql->SelectMySql(sqlcmd,1,res);
	bzero(sqlcmd,200);
	if(res.empty()){
		//no rpeat
		re.m_res=_DEF_REGISTER_OK;	
		//update mysql
		sprintf(sqlcmd,"insert into t_userinfo(Name,pswd,tel,icon) value('%s','%s','%s','0');",msg->m_reg.m_name,msg->m_reg.m_pswd,msg->m_reg.m_tel);
		if(0==m_pSql->UpdateMySql(sqlcmd)){
			cout<<"mysql insert err"<<endl;
			re.m_res=_DEF_REGISTER_OTHRE;
			cout<<"name: "<<msg->m_reg.m_name<<endl<<"pswd: "<<msg->m_reg.m_pswd<<endl<<"tel: "<<msg->m_reg.m_tel<<endl;
		}
	}else{
		if(0==strcmp(msg->m_reg.m_tel,res.front().c_str())){
			re.m_res=_DEF_REGISTER_TELUSERD;
		}else{
			re.m_res=_DEF_REGISTER_USEREXIST;
		}
	}
	//reply
	send((char*)&re,sizeof(Reply),from);
}
void CEpollServer::send_userinfo(int userid,int aim){
	//get info from mysql depend on user id
	char sqlcmd[100]={0};
	sprintf(sqlcmd,"select Name,icon,sign,ID from t_userinfo where ID='%d';",userid);
	list<string> res;
	m_pSql->SelectMySql(sqlcmd,4,res);
	if(res.empty()){
		cout<<"get "<<userid<<" info err"<<endl;
	}else{
		Reply re;
		re.m_type=_DEF_TCP_USER;
		UserInfo* info=get_playerinfo(userid);
		re.data.m_userinfo=*info;		
		delete info;
		send((char*)&re,sizeof(Reply),aim);

	}


}

void CEpollServer::send_friends(int userid,int aim){

}

void CEpollServer::deal_joinzone(char* data,int from){
	Msg_JoinZone* joinzone=(Msg_JoinZone*)data;
	int playid=joinzone->m_zone.m_playerid;
	int zoneid=joinzone->m_zone.m_zoneid;
	Reply re;
	re.m_type=_DEF_TCP_JOINZONE;
	if(m_veczones[zoneid]->add_player(playid,m_mpIdtoSock[playid])){
		re.m_res=_DEF_ZONE_JOINFAIL;
		cout<<"join zone fail"<<endl;
	}else{
		cout<<"join zone ok"<<endl;
		re.m_res=_DEF_ZONE_JOINOK;
		re.data.m_zoneinfo.m_playerid=playid;
		re.data.m_zoneinfo.m_zoneid=zoneid;
	}
	send((char*)&re,sizeof(Reply),from);
}
void CEpollServer::deal_leavezone(char* data,int from){
	Msg_LeaveZone* leavezone=(Msg_LeaveZone*)data;
	int playid=leavezone->m_zone.m_playerid;
	int zoneid=leavezone->m_zone.m_zoneid;
	m_veczones[zoneid]->del_player(playid);
	Reply re;
	re.m_type=_DEF_TCP_LEAVEZONE;
	re.m_res=_DEF_ZONE_LEAVEOK;
	send((char*)&re,sizeof(Reply),from);

}
void CEpollServer::deal_createroom(char* data,int from){
	Msg_CreateRoom* cr=(Msg_CreateRoom*)data;
	int userid=cr->m_userid;
	int zoneid=cr->m_zoneid;
	int res=m_veczones[zoneid]->add_room(userid);
	Reply re;
	if(res>=0){
		re.m_type=_DEF_TCP_JOINROOM;
		re.m_res=_DEF_ROOM_JOINOK;
		re.data.m_roomid.m_roomid=res;
		send_gameroominfo(zoneid,res);

	}else{
		re.m_type=_DEF_TCP_CREATEROOM;
	}	
	send((char*)&re,sizeof(Reply),from);
}
void CEpollServer::deal_joinroom(char* data,int from){
	Msg_JoinRoom* jr=(Msg_JoinRoom*)data;
	int roomid=jr->m_roomid;
	int userid=jr->m_userid;
	int zoneid=jr->m_zoneid;
	CRoom* rm=m_veczones[zoneid]->get_room(roomid);
	if(rm){
		Reply re;
		re.m_type=_DEF_TCP_JOINROOM;
		if(rm->iscanbejoin()){
			int icon=get_playericon(userid);
			int usersock=get_playersock(userid);
			rm->add_player(userid,usersock,icon);
			re.m_res=_DEF_ROOM_JOINOK;
			re.data.m_roomid.m_roomid=roomid;
			send_gameroominfo(zoneid,roomid);
		}else re.m_res=_DEF_ROOM_JOINFAIL;
		send((char*)&re,sizeof(Reply),from);
	}

}
void CEpollServer::deal_leaveroom(char* data,int from){
	Msg_LeaveRoom* lr=(Msg_LeaveRoom*)data;
	int roomid=lr->m_roomid;
	int userid=lr->m_userid;
	int zoneid=lr->m_zoneid;
	CRoom* rm=m_veczones[zoneid]->get_room(roomid);
	if(rm){
		rm->del_player(rm->get_roomid(userid));
		m_veczones[zoneid]->del_room(roomid);
		Reply re;
		re.m_type=_DEF_TCP_LEAVEROOM;
		send((char*)&re,sizeof(Reply),from);	
		send_gameroominfo(zoneid,roomid);
	}	
}
void CEpollServer::deal_refreshrooms(char* data,int from){
	Msg_RefreshRooms* msg=(Msg_RefreshRooms*)data;
	int zoneid=msg->m_zoneid;
	cout<<"room refresh in zone "<<zoneid<<endl;
	CZone* zone=m_veczones[zoneid];
	zone->msg_roomrefreshtoone(from);


}
void CEpollServer::send_gameroominfo(int zoneid,int roomid){
	CRoom* rm=m_veczones[zoneid]->get_room(roomid);
	cout<<"refresh game room"<<endl;
	if(rm){
		Reply re;
		re.m_type=_DEF_TCP_GAMEROOM;
		rm->load_roominfo(&re.data.m_gameroominfo);
		int id0=rm->get_playerid(0);
		int id1=rm->get_playerid(1);
		if(id0>=0){
			int sock=get_playersock(id0);

			if(sock>0){
				re.data.m_gameroominfo.m_color=1;
				send((char*)&re,sizeof(Reply),sock);
			}
		}
		if(id1>=0){

			int sock=get_playersock(id1);
			char temp[_DEF_USER_LEN*2]={0};
			strcpy(temp,re.data.m_gameroominfo.m_name[0]);
			strcpy(re.data.m_gameroominfo.m_name[0],re.data.m_gameroominfo.m_name[1]);

			strcpy(re.data.m_gameroominfo.m_name[1],temp);
			if(sock>0){
				re.data.m_gameroominfo.m_color=2;
				send((char*)&re,sizeof(Reply),sock);
			}

		}
	}			
}
void CEpollServer::deal_piecedown(char* data,int from){
	cout<<__func__<<endl;
	Msg_PieceDown* piecedown=(Msg_PieceDown*)data;
	int roomid=piecedown->piecedown.m_roomid;
	int zoneid=piecedown->piecedown.m_zoneid;
	int color=piecedown->piecedown.m_color;
	if(color<=0||color>2) return;
	Reply re;
	re.m_type=_DEF_TCP_PIECEDOWN;
	re.data.m_piecedown=piecedown->piecedown;
	CRoom* rm=m_veczones[zoneid]->get_room(roomid);
	int id=rm->get_playerid(color%2);
	if(id<0){
		cout<<id<<" id err"<<endl;
	}
	int sock=m_mpIdtoSock[id];
	if(sock<0){
		cout<<sock<<" sock err"<<endl;
	}
	send((char*)&re,sizeof(Reply),sock);
		
}
void CEpollServer::deal_win(char* data,int from){
	
	Msg_Win* win=(Msg_Win*)data;
	
	int roomid=win->m_roomid;
	int zoneid=win->m_zoneid;
	int color=win->m_color;
	if(color<0||color>2) return;
	Reply re;
	re.m_type=_DEF_TCP_WIN;
	re.data.m_win.m_color=color;
	CRoom* rm=m_veczones[zoneid]->get_room(roomid);
	int id0=rm->get_playerid(0);
	int id1=rm->get_playerid(1);
	if(id0<0||id1<0) return;
	int sock0=m_mpIdtoSock[id0];
	int sock1=m_mpIdtoSock[id1];
	if(sock0<0||sock1<0) return;
	send((char*)&re,sizeof(Reply),sock0);
	send((char*)&re,sizeof(Reply),sock1);
		
	
}
void CEpollServer::deal_winact(char* data,int from){
}
void CEpollServer::deal_startgame(char* data,int from){
	cout<<__func__<<endl;
	Msg_StartGame* sg=(Msg_StartGame*)data;
	int roomid=sg->m_roomid;
	int zoneid=sg->m_zoneid;
	int re=_DEF_TCP_STARTGAME;
	CRoom* rm=m_veczones[zoneid]->get_room(roomid);
	if(rm){
		int id0=rm->get_playerid(0);
		int id1=rm->get_playerid(1);
		if(id0<0||id1<0){
			cout<<"id error"<<endl;
			return;
		} 
		int sock0=m_mpIdtoSock[id0];
		int sock1=m_mpIdtoSock[id1];
		if(sock0<0||sock1<0){
			cout<<"sock err"<<endl;
		}
		send((char*)&re,sizeof(int),sock0);
		send((char*)&re,sizeof(int),sock1);
	}
			
}
