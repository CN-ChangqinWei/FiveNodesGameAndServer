#include"../inc/tcpserver.h"



Ctcpserver::Ctcpserver():m_sock(-1){
	
	m_sock=init();
}
Ctcpserver::~Ctcpserver(){


}
int Ctcpserver::init(){
	int sock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));		
	addr.sin_family=AF_INET;
	addr.sin_port=htons(_DEF_PORT);
	addr.sin_addr.s_addr=htonl(_DEF_IP);
	//addr.sin_addr.s_addr=inet_addr("192.168.53.133");
	if(-1==bind(sock,(struct sockaddr*)&addr,sizeof(addr))){
		perror("bind fail");
		return -1;
	}
	if(-1==listen(sock,128)){
		perror("listen fail");
		return -1;
	}
	return sock;
}
void Ctcpserver::close(){
	::close(m_sock);
	m_sock=-1;

}
int Ctcpserver::send(char* data,int len,int sockto){
	if(sockto<0) return -1;
	
	if(::send(sockto,(char*)&len,sizeof(int),MSG_NOSIGNAL)<=0){
		perror("send fail");
		::close(sockto);
		return -1;
	}
	if(::send(sockto,data,len,MSG_NOSIGNAL)<=0){
		perror("send fail");
		::close(sockto);
		return -1;
	}
	return 0;
}
int Ctcpserver::recv(char* buf,int len,int sockfrm){
	
	if(sockfrm<0) return -1;

	if(::recv(sockfrm,(char*)&len,sizeof(int),MSG_NOSIGNAL)<=0)
	{
		perror("recv len fail");
		::close(sockfrm);
		return -1;
	}
	if(::recv(sockfrm,buf,len,MSG_NOSIGNAL)<=0)	
	{
		perror("recv data fail");
		::close(sockfrm);
		return -1;
	}
	return 0;
}	
int Ctcpserver::acp(struct sockaddr* addr,socklen_t size){
	
	if(nullptr==addr) return -1;
	int sock;
	sock=accept(m_sock,addr,&size);
	if(sock<0){
		perror("accept fail");
		return -1;
	}

	return sock;
}
int Ctcpserver::getsock(){
	return m_sock;
}
	
