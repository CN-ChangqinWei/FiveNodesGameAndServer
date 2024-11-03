#include<iostream>
#include"../inc/threadspoll.h"
#include"../inc/epollserver.h"
using namespace std;
int times=0;
void* thread_job(void* arg){
	long long num=(long long)arg;
	times++;
	cout<<hex<<pthread_self()<<"    "<<dec<<num<<"excute  "<<times<<endl;
	return 0;
}

int main(){
	CEpollServer::CreateServer();
	CEpollServer::server->init();
	while(1) sleep(1);	


	return 0;
}
