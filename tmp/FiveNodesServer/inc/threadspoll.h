#ifndef _THREADSPOLL_H
#define _THREADSPOLL_H
#include<iostream>
#include<list>
#include<map>
#include<queue>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<pthread.h>
using namespace std;

struct task_t{
	void* (*task)(void* arg);
	void *arg;
	task_t(){
		task=nullptr;
		arg=NULL;

	}
};
void* custom_func(void* arg);
void* manager_func(void* arg);
class CThreadspoll{
public:
	CThreadspoll(int max,int min,int taskmax);
	~CThreadspoll();
	bool init(int max,int min,int taskmax);
	void close();
	void killall();
	void killnumfree(int num);
	void killnum(int num);
	void killonefree();
	int get_threadsmax();
	int get_threadsmin();
	int get_threadsnum();
	int get_freenum();
	int get_busynum();
	int get_tasknum();
	bool get_readyflag();
	bool get_shutdown();
	bool get_suicide();
	void ifree();
	void ibusy();
	void ifreedbusy();
	void ibusydfree();
	void dfree();
	pthread_t create_onethread();
	int create_mutithreads(int num);
	void thread_sleep();
	void thread_awake();
	bool thread_gettask(task_t* ptask);
	void task_add(void* (*task)(void*),void *arg);
	task_t task_get(); 
	pthread_mutex_t m_lock;
	pthread_cond_t m_cond;
	//list<pthread_t> lst_threads;
private:
	map<pthread_t,bool> m_mpbusy;
	map<pthread_t,bool> m_mpfree;
	queue<task_t> m_quetask;
	pthread_t m_manager;
	int m_threadmax;
	int m_threadmin;
	int m_taskmax;
	bool m_readyflag;
	bool m_shutdown;
	bool m_suicide;
	int m_free;
	int m_busy;
};

#endif
