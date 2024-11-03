#ifndef _SERVER_MAP_H
#define _SERVER_MAP_H
class CEpollServer;
void *func_recvmsg(void *arg);
void *func_dealmsg(void *arg);
void *func_accept(void* arg);

//业务
void *func_show(void *arg);//服务器显示接受函数	
void *func_time(void* arg);//返回当前时间
void *func_login(void* arg);//登入请求
void *func_register(void* arg);//注册请求
void *func_joinzone(void* arg);//join zone
void *func_leavezone(void*arg);//leave zone
void *func_createroom(void* arg);//create room
void *func_joinroom(void* arg);//join room
void *func_leaveroom(void* arg);//leave room
void *func_refreshrooms(void* arg);
void *func_piecedown(void* arg);
void *func_win(void* arg);
void *func_winact(void* arg);
void *func_startgame(void* arg);
#endif
