#include"../inc/threadspoll.h"


void* custom_func(void* arg){
	pthread_detach(pthread_self());
	CThreadspoll* poll=(CThreadspoll*)arg;
	pthread_mutex_lock(&poll->m_lock);
	cout<<"create thread"<<hex<<pthread_self()<<endl;
	task_t task;
	while(1){
		if(poll->get_suicide()){
			cout<<hex<<pthread_self()<<" 线程退出"<<endl;
			pthread_mutex_unlock(&poll->m_lock);
			break;
		}
		if(poll->get_shutdown()){
			if(poll->get_tasknum()){
				if(0==poll->thread_gettask(&task)){
					pthread_mutex_unlock(&poll->m_lock);
					(*task.task)(task.arg);
					pthread_mutex_lock(&poll->m_lock);	
					poll->thread_sleep();
				}else{
					
					pthread_cond_wait(&poll->m_cond,&poll->m_lock);
				}
			//	cout<<hex<<pthread_self()<<"苏醒"<<endl;
			}else{
			//	cout<<"无任务"<<endl;
			 	pthread_cond_wait(&poll->m_cond,&poll->m_lock);
			//	cout<<hex<<pthread_self()<<"苏醒"<<endl;
			}
			
		}else{
			pthread_cond_wait(&poll->m_cond,&poll->m_lock);
			//	cout<<hex<<pthread_self()<<"苏醒"<<endl;
		}		
	}
	return 0;
}

void* manager_func(void* arg){
	pthread_detach(pthread_self());
	CThreadspoll* poll=(CThreadspoll*)arg;
	cout<<"create manager thread"<<hex<<pthread_self()<<endl;
	pthread_mutex_lock(&poll->m_lock);	
//	poll->m_manager=pthread_self();
	pthread_mutex_unlock(&poll->m_lock);
	
	while(1){
		pthread_mutex_lock(&poll->m_lock);
		cout<<"线程数："<<poll->get_threadsnum()<<"忙碌："<<poll->get_busynum()	<<"空闲："<<poll->get_freenum()<<"剩余任务："<<poll->get_tasknum()<<endl;	
		while(poll->get_freenum()&&poll->get_freenum()>poll->get_threadsmin()){
			//poll->dfree();
			poll->killonefree();
			pthread_cond_signal(&poll->m_cond);
			pthread_mutex_unlock(&poll->m_lock);
			usleep(10);
			pthread_mutex_lock(&poll->m_lock);			
		}
		pthread_mutex_unlock(&poll->m_lock);
		sleep(1);
	}
	return 0;
}

CThreadspoll::CThreadspoll(int max,int min,int taskmax):m_suicide(false),m_free(0),m_busy(0),m_readyflag(0),m_threadmax(0),m_threadmin(0),m_taskmax(0),m_shutdown(1)
{
	pthread_mutex_init(&m_lock,NULL);
	pthread_cond_init(&m_cond,NULL);
	init(max,min,taskmax);
	
}
CThreadspoll::~CThreadspoll(){
	
}
bool CThreadspoll::init(int max,int min,int taskmax){
	if(max<=0||min<=0||taskmax<=0){
		cout<<__func__<<"初始化参数小于等于零"<<endl;
		return -1;
	}	
	m_threadmax=max;
	m_free=m_threadmin=min;
	m_taskmax=taskmax;
	pthread_t tid;
	//pthread_create()
	for(int i=0;i<m_threadmin;i++){
		pthread_mutex_lock(&m_lock);
		pthread_create(&tid,NULL,custom_func,this);
		//m_mpfree[tid]=true;
		pthread_mutex_unlock(&m_lock);
		usleep(10);
	}
	pthread_mutex_lock(&m_lock);
	//for(auto v:m_mpfree) cout<<v.first<<endl;
	pthread_create(&tid,NULL,manager_func,this);
	m_manager=tid;
	pthread_mutex_unlock(&m_lock);
	return 0;
}
void CThreadspoll::close(){
	
}

void CThreadspoll::killall(){

}
void CThreadspoll::killnumfree(int num){

}
void CThreadspoll::killnum(int num){

}
void CThreadspoll::killonefree(){

	if(m_free){
		m_free--;
		m_suicide=true;
	}
}
int CThreadspoll::get_threadsmin(){

	return m_threadmin;
}
int CThreadspoll::get_threadsnum(){
	return m_free+m_busy;
}
int CThreadspoll::get_freenum(){
	return m_free;
}
int CThreadspoll::get_busynum(){
	return m_busy;
}
int CThreadspoll::get_tasknum(){


	return m_quetask.size();
}
bool CThreadspoll::get_readyflag(){
	return m_readyflag;
}
bool CThreadspoll::get_shutdown(){
	return m_shutdown;
}

bool CThreadspoll::get_suicide()
{
	if(m_suicide){
		m_suicide=false;
		return true;
	}
	return false;
}

void CThreadspoll::ifree(){
	m_free++;

}
void CThreadspoll::ibusy(){
	m_busy++;
}
void CThreadspoll::ifreedbusy(){
	m_free++;
	m_busy--;
}
void CThreadspoll::ibusydfree(){
	m_busy++;
	m_free--;
}
void CThreadspoll::dfree(){
	m_free--;

}
pthread_t CThreadspoll::create_onethread(){
	pthread_t tid=0;



	return tid;
}
int CThreadspoll::create_mutithreads(int num){
	int create_num=0;
	
	return create_num;
}
void CThreadspoll::thread_sleep(){
	//if(m_mpbusy.end()!=m_mpbusy.find(pthread_self()))
	//	m_mpbusy.erase(pthread_self());
	//m_mpfree[pthread_self()]=true;
	m_free++;
	m_busy--;	
	pthread_cond_wait(&m_cond,&m_lock);
}
void CThreadspoll::thread_awake(){
	//m_mpfr.erase()
}
bool CThreadspoll::thread_gettask(task_t* ptask){
	
	if(ptask&&m_quetask.size()){
		*ptask=m_quetask.front();
		m_quetask.pop();	
		//m_mpbusy[pthread_self()]=true;
		
		//if(m_mpfree.end()!=m_mpfree.find(pthread_self()))
			//m_mpfree.erase(pthread_self());
		//m_busy++;
		return 0;		
	}
	cout<<__func__<<"task ptr is null or there is no tasks"<<endl;
	
	return -1;
}
void CThreadspoll::task_add(void* (*task)(void*),void* arg){
	if(task){
		pthread_mutex_lock(&m_lock);
		cout<<"添加任务"<<endl;
		task_t tk;
		tk.task=task;
		tk.arg=arg;
		m_quetask.push(tk);
		cout<<m_quetask.size()<<endl;
		if(m_free){
			pthread_cond_signal(&m_cond);
			m_free--;
			m_busy++;
			sleep(0);				
		}else{
			pthread_t tid;
			pthread_create(&tid,0,custom_func,this);
			m_busy++;
			m_mpbusy[tid]=true;			
		}	
		pthread_mutex_unlock(&m_lock);
		//sleep(0);
	}
}
task_t CThreadspoll::task_get(){
	
	task_t task;
	if(m_quetask.empty()) return task;
	task=m_quetask.front();
	//m_mpbusy[pthread_self()]=true;
	//m_mpfree.erase(pthread_self());
	return task;
}
