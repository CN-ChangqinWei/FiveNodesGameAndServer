#ifndef _ROOM_H
#define _ROOM_H
#include<map>
#include<vector>
#include"protocl.h"
#define _DEF_FIVENODES	(0)
#define _DEF_FIVENODES_NOFPLAYERS	(2)
using namespace std;
struct PlayerInfo{
	int id;
	int icon;
	int sock;
	PlayerInfo(){
		id=-1;
		icon=-1;
		sock=-1;
	}
};
class CRoom{
public:
	CRoom(int gameid,int RoomID,int hostid);
	~CRoom();
	void init(int hostid);
	void close();
	//player operation
	bool add_player(int id,int sock,int icon);
	void del_player(int roomid);
	void tick_player(int roomid);	
	//room info;
	int get_gameid();
	int get_playernum();
	int get_maxplayers();
	int get_Stat();
	int get_roomid(int userid);
	int get_playerid(int roomcode);	

	void del_playerbyuserid(int userid);
	void load_reply(Reply* re);
	void load_roominfo(GameRoomInfo* info);
	bool isexist();
	bool iscanbejoin();
	

private:
	int m_GameID;
	int m_nMaxPlayers;
	int m_nPlayerNums;
	int m_Stat;
	int m_RoomID;
	vector<PlayerInfo> m_vecplayers;	
	map<int,int> m_mpplayersock;
	
};
#endif
