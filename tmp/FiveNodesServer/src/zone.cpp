#include"../inc/zone.h"
#include"../inc/epollserver.h"
#include"protocl.h"
#include<list>
CZone::CZone(int gameid,int maxrooms){
	init(gameid,maxrooms);
}
CZone::~CZone(){

}
void CZone::init(int gameid,int maxrooms){
	m_GameID=gameid;
	m_nMaxRooms=maxrooms;
	m_mproominfo.resize(maxrooms,nullptr);	
	m_nRooms=0;
}
void CZone::close(){

}
//room operation
int CZone::add_room(int userid){
	lock_guard<mutex> lg(m_lock);
	for(int i=0;i<m_mproominfo.size();i++){
		if(nullptr==m_mproominfo[i]){
			m_mproominfo[i]=new CRoom(0,i,userid);
			return i;
		} 
	}
	return -1;
}
void CZone::del_room(int id){
	lock_guard<mutex> lg(m_lock);
	if(m_mproominfo[id]&&!m_mproominfo[id]->isexist()){
		delete m_mproominfo[id];
		m_mproominfo[id]=nullptr;
	}
}
bool CZone::add_roomplayer(int roomid,int playerid){
	lock_guard<mutex> lg(m_lock);
	return false;
}		
//player operation
bool CZone::add_player(int id,int sock){
	lock_guard<mutex> lg(m_lock);
	if(m_mpplaysock.end()!=m_mpplaysock.find(id)){
		cout<<id<<" player exist in zone "<<m_GameID<<endl;
		return true;
	}else{
		m_mpplaysock[id]=sock;
		cout<<id<<" player join the zone "<<m_GameID<<endl;
	}
	return false;
}
void CZone::del_player(int id){
	lock_guard<mutex> lg(m_lock);
	if(m_mpplaysock.end()!=m_mpplaysock.find(id)){
		cout<<id<<" player leave zone "<<endl;
		m_mpplaysock.erase(m_mpplaysock.find(id));
	}
}
//msg operation
void CZone::msg_sendtoplayer(){
	lock_guard<mutex> lg(m_lock);

}
void CZone::msg_boast(){
	lock_guard<mutex> lg(m_lock);

}
void CZone::msg_roomrefresh(){
	lock_guard<mutex> lg(m_lock);
	list<Reply*> lstRe;
	Reply* re=new Reply;
	re->m_type=_DEF_TCP_REFRESHROOM;
	re->m_res=_DEF_ROOMREFRESH_BEGIN;
	lstRe.push_back(re);
	for(int i=0;i<m_mproominfo.size();i++){
		if(m_mproominfo[i]){
			re=new Reply;
			m_mproominfo[i]->load_reply(re);
			lstRe.push_back(re);			
		}
	}
	re=new Reply;
	re->m_type=_DEF_TCP_REFRESHROOM;
	re->m_res=_DEF_ROOMREFRESH_END;
	lstRe.push_back(re);
	int sock;
	for(pair<int,int> p:m_mpplaysock){	
		for(Reply* v:lstRe){
			_DEF_SERVER->send((char*)v,sizeof(Reply),p.second);	
		}	
	}	
	for(Reply* v:lstRe){
		delete v;
	}	

}

void CZone::msg_roomrefreshtoone(int sock){

	lock_guard<mutex> lg(m_lock);
	list<Reply*> lstRe;
	Reply* re=new Reply;
	re->m_type=_DEF_TCP_REFRESHROOM;
	re->m_res=_DEF_ROOMREFRESH_BEGIN;
	lstRe.push_back(re);
	for(int i=0;i<m_mproominfo.size();i++){
		if(m_mproominfo[i]){
			re=new Reply;
			m_mproominfo[i]->load_reply(re);
			lstRe.push_back(re);			
		}
	}
	re=new Reply;
	re->m_type=_DEF_TCP_REFRESHROOM;
	re->m_res=_DEF_ROOMREFRESH_END;
	lstRe.push_back(re);
	for(Reply* v:lstRe){
		_DEF_SERVER->send((char*)v,sizeof(Reply),sock);
	}

	for(Reply* v:lstRe){
		delete v;
	}	
}


bool CZone::joinroom(int userid,int roomid){
	m_mpplayertoroom[userid]=roomid;
}
bool CZone::leaveroom(int userid){
	if(m_mpplayertoroom.find(userid)!=m_mpplayertoroom.end()){
		int roomid=m_mpplayertoroom[userid];
		m_mpplayertoroom.erase(m_mpplayertoroom.find(userid));
		CRoom* rm=m_mproominfo[roomid];
		rm->del_playerbyuserid(userid);		
	}
}

CRoom* CZone::get_room(int roomid){
	return m_mproominfo[roomid];
}	
