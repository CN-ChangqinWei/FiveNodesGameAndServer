#include"../inc/room.h"
#include<iostream>
#include"epollserver.h"
using namespace std;
CRoom::CRoom(int gameid,int RoomID,int hostid):m_GameID(gameid),m_RoomID(RoomID){
	init(hostid);
}
CRoom::~CRoom(){

}
void CRoom::init(int hostid){

	if(m_GameID==0){
		m_nMaxPlayers=_DEF_FIVENODES_NOFPLAYERS;
	}
	for(int i=0;i<m_nMaxPlayers;i++){
		m_vecplayers.push_back(PlayerInfo());
	}
	int hosticon=_DEF_SERVER->get_playericon(hostid);
	int hostsock=_DEF_SERVER->get_playersock(hostid);
	add_player(hostid,hostsock,hosticon);

}
void CRoom::close(){

}
//player operation
bool CRoom::add_player(int id,int sock,int icon){
	for(int i=0;i<2;i++){
		if(m_vecplayers[i].icon<0){
			m_nPlayerNums++;
			m_vecplayers[i].id=id;
			m_vecplayers[i].sock=sock;
			m_vecplayers[i].icon=icon;
			cout<<"player "<<id<<" join room "<<m_RoomID<<endl;	
			return false;
		
		}
	}	
	cout<<"room "<<m_RoomID<<" full"<<endl;
	return true;
}
void CRoom::del_player(int roomid){
	if(roomid<0||roomid>1) return;
	if(m_vecplayers[roomid].id<0) return;
	else{
		m_nPlayerNums--;
		m_vecplayers[roomid].id=-1;
		m_vecplayers[roomid].icon=-1;
		m_vecplayers[roomid].sock=-1;
		//m_nPlayerNums--;
		cout<<m_RoomID<<" room delete "<<roomid<<endl;
	}
	if(m_vecplayers[1].id>=0){
		m_vecplayers[0]=m_vecplayers[1];
		m_vecplayers[1].id=-1;
		m_vecplayers[1].sock=-1;
		m_vecplayers[1].icon=-1;
	}

}
void CRoom::tick_player(int roomid){

}
//room info;
int CRoom::get_gameid(){

}
int CRoom::get_playernum(){
	return m_nPlayerNums;
}
int CRoom::get_maxplayers(){

}
int CRoom::get_Stat(){

}
int CRoom::get_roomid(int userid){
	for(int i=0;i<2;i++){
		if(m_vecplayers[i].id==userid) return i;	
	}
	return -1;
}
void CRoom::load_reply(Reply* re){
	re->m_type=_DEF_TCP_REFRESHROOM;
	re->m_res=_DEF_ROOMREFRESH_TRANS;
	for(int i=0;i<2;i++){
		re->data.m_roominfo.m_icons[i]=m_vecplayers[i].icon;
	}
	re->data.m_roominfo.m_roomid=m_RoomID;

}

bool CRoom::isexist(){
	for(int i=0;i<2;i++){
		if(m_vecplayers[i].id>=0) return true;
	}
	return false;
}
bool CRoom::iscanbejoin(){
	for(int i=0;i<2;i++){
		if(m_vecplayers[i].id<0) return true;
	}
	return false;
}
void CRoom::del_playerbyuserid(int userid){
	 
	for(int i=0;i<2;i++){
		if(userid==m_vecplayers[i].id){
	
		del_player(i);
		break;
	}
}

}

void CRoom::load_roominfo(GameRoomInfo* info){
	UserInfo* users[2]={0};
	for(int i=0;i<2;i++){
		if(m_vecplayers[i].id>=0){
			users[i]=_DEF_SERVER->get_playerinfo(m_vecplayers[i].id);
		}

	}
	memset(info,0,sizeof(GameRoomInfo));
	info->m_icons[0]=-1;
	info->m_icons[1]=-1;
	for(int i=0;i<2;i++){
		if(users[i]){
			info->m_icons[i]=users[i]->icon;
			strcpy(info->m_name[i],users[i]->m_name);
			delete users[i];
		}
		
	}






}
int CRoom::get_playerid(int roomcode){
	if(roomcode<0||roomcode>1) return -1;
	return m_vecplayers[roomcode].id;

}	

