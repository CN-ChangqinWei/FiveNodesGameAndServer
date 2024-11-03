#include"../inc/servermap.h"
#include"../inc/epollserver.h"
void* func_recvmsg(void* arg){
	//pthread_detach(pthread_self());
	//CEpollServer* epoll=(CEpollServer*)arg;
	_FUNC_SHOW(0);	
	int sock=(long long)arg;
	char buf[4096]={0};
	if(CEpollServer::server->recv(buf,4096,sock)){
		return 0;
	}
	CEpollServer::server->addlst(sock);	
	//CEpollServer::server->deal(buf,4096,sock);
	cout<<buf<<endl;
	return 0;
}

void* func_accept(void* arg){
	
	CEpollServer::server->acp();
	return 0;
}

void* func_dealmsg(void* arg){
	cout<<"开始接收"<<endl;
	int clientsock=(long long)arg;
	char* buf=new char[4096];		
	if(CEpollServer::server->recv(buf,4096,clientsock)){
		return 0;
	}//else CEpollServer::server->deal(buf,4096,clientsock);
	CEpollServer::server->addlst(clientsock);
	cout<<"接受完毕"<<endl;
	return 0;
}

void* func_show(void* arg){
	cout<<"show"<<endl;
	DealMsg* dealmsg=(DealMsg*)arg;
	cout<<dealmsg->data<<endl;
	//char text[1024]={0};
	char* text=(char*)malloc(4096);
	sprintf(text,"recv:%s",dealmsg->data);
	CEpollServer::server->send(text,strlen(text),dealmsg->m_sFrom);
	delete []dealmsg->data;
	delete dealmsg;
	delete []text;
	return 0;
}

void* func_time(void* arg){
	cout<<"time"<<endl;
	DealMsg* dealmsg=(DealMsg*)arg;
	Reply re;
	re.m_type=_DEF_TCP_TIME;
	time_t thetime;
	time(&thetime);
	const char* str=ctime(&thetime);
	cout<<str<<endl;			
	strcpy(re.data.m_time.content,str);
	CEpollServer::server->send((char*)&re,sizeof(re),dealmsg->m_sFrom);	
	return 0;
}


void *func_login(void* arg){
	_FUNC_EXCUTE(login)	

	return nullptr;
}//登入请求
void *func_register(void* arg){
	_FUNC_EXCUTE(register)
	return nullptr;
}//注册请求

void *func_joinzone(void* arg){
	
	_FUNC_EXCUTE(joinzone)
	return nullptr;

}
void *func_leavezone(void* arg){

	_FUNC_EXCUTE(leavezone)
	return nullptr;
}
void *func_createroom(void* arg){
	_FUNC_EXCUTE(createroom)
	return nullptr;

}
void *func_joinroom(void* arg){
	_FUNC_EXCUTE(joinroom)
	return nullptr;

}

void *func_leaveroom(void* arg){
	_FUNC_EXCUTE(leaveroom)
	return nullptr;

}
void *func_refreshrooms(void* arg){
	_FUNC_EXCUTE(refreshrooms)
	return nullptr;
}
void *func_piecedown(void* arg){
	_FUNC_EXCUTE(piecedown)
	return nullptr;
}
void *func_win(void* arg){
	_FUNC_EXCUTE(win)
	return nullptr;
}
void *func_winact(void *arg){
	_FUNC_EXCUTE(winact);
	return nullptr;
}
void *func_startgame(void* arg){
	_FUNC_EXCUTE(startgame)
	return nullptr;
}




