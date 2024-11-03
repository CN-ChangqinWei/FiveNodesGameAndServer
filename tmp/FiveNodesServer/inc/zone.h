#ifndef _ZONE_H
#define _ZONE_H
#include<map>
#include"room.h"
#include<vector>
#include<mutex>
using namespace std;
class CZone{
public:
	CZone(int gameid,int maxrooms);
	~CZone();
	void init(int gameid,int maxrooms);
	void close();
	//room operation
	int add_room(int userid);
	void del_room(int id);
	bool add_roomplayer(int roomid,int playerid);
	CRoom* get_room(int roomid);	
		
	//player operation
	bool add_player(int id,int sock);
	void del_player(int id);
	bool joinroom(int userid,int roomid);
	bool leaveroom(int userid);

	//msg operation
	void msg_sendtoplayer();
	void msg_boast();
	void msg_roomrefresh();
	void msg_roomrefreshtoone(int sock);
private:
//player id-socks
	map<int,int> m_mpplaysock;
	map<int,int> m_mpplayertoroom;
//rooms id-info
	vector<CRoom*> m_mproominfo;
	
	int m_nMaxRooms;
	int m_GameID;
	int m_nRooms;
	mutex m_lock;
};






#endif

