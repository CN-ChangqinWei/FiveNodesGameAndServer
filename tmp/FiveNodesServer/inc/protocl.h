#ifndef _PROTOCL_H
#define _PROTOCL_H
#include<string.h>

#define _DEF_MAXROOMS  (128)

#define _DEF_LEN_DATA	(30)

#define _DEF_TEL_LEN    (11)

#define _DEF_USER_LEN   (25)

#define _DEF_PSWD_LEN   (20)

#define _DEF_SIGN_LEN   (49)

#define _DEF_NOF_FUNCS	(20)

#define _DEF_NOF_GAMES	(5)

//TCP Tasks
#define _DEF_TCPBASE	(1000)

#define _DEF_TCP_SHOW	(_DEF_TCPBASE+0)

#define _DEF_TCP_TIME	(_DEF_TCPBASE+1)

#define _DEF_TCP_LOGIN  (_DEF_TCPBASE+2)

#define _DEF_TCP_REGISTER   (_DEF_TCPBASE+3)

#define _DEF_TCP_USER	(_DEF_TCPBASE+4)

#define _DEF_TCP_JOINZONE	(_DEF_TCPBASE+5)

#define _DEF_TCP_LEAVEZONE	(_DEF_TCPBASE+6)

#define _DEF_TCP_CREATEROOM	(_DEF_TCPBASE+7)

#define _DEF_TCP_JOINROOM	(_DEF_TCPBASE+8)

#define _DEF_TCP_LEAVEROOM	(_DEF_TCPBASE+9)

#define _DEF_TCP_REFRESHROOM	(_DEF_TCPBASE+10)

#define _DEF_TCP_GAMEROOM       (_DEF_TCPBASE+11)

#define _DEF_TCP_PIECEDOWN      (_DEF_TCPBASE+12)

#define _DEF_TCP_WIN            (_DEF_TCPBASE+13)

#define _DEF_TCP_WINACT         (_DEF_TCPBASE+14)

#define _DEF_TCP_STARTGAME      (_DEF_TCPBASE+15)

//TIME EVENT

#define _DEF_TIME_OK	(0)

#define _DEF_TIME_FAIL	(1)

//LOGIN EVENT

#define _DEF_LOGIN_OK	(0)

#define _DEF_LOGIN_USERNOTEXIST	(1)

#define _DEF_LOGIN_PSWDERR	(2)

#define _DEF_LOGIN_OTHER	(3)

#define _DEF_LOGIN_EXIST	(4)

//REGISTER EVENT

#define _DEF_REGISTER_OK	(0)

#define _DEF_REGISTER_USEREXIST	(1)

#define _DEF_REGISTER_TELUSERD	(2)

#define _DEF_REGISTER_OTHRE	(3)
//ZONE EVENT

#define _DEF_ZONE_JOINOK	(0)

#define _DEF_ZONE_JOINFAIL	(1)

#define _DEF_ZONE_LEAVEOK	(0)

//ROOM EVENT

#define _DEF_ROOM_JOINOK	(0)

#define _DEF_ROOM_JOINFAIL	(1)


//ROOM REFRESH
#define _DEF_ROOMREFRESH_BEGIN	(0)

#define _DEF_ROOMREFRESH_TRANS	(1)

#define _DEF_ROOMREFRESH_END	(2)

//GameRoom Stat
#define _DEF_GAMEROOM_PRE       (false)

#define _DEF_GAMEROOM_FIGHT     (true)

//Win Act
#define _DEF_WINACT_STAY	(0)

#define _DEF_WINACT_LEAVE	(1)

struct ShowMsg{
	char data[_DEF_LEN_DATA];/*
	Msg(){
		memset(data,0,sizeof(_DEF_LEN_DATA));
	}*/
};
struct TimeRe{
    char content[_DEF_LEN_DATA];
};
struct Login{
    char m_tel[_DEF_TEL_LEN+1];
    char m_pswd[33];
};
struct Register{
    char m_tel[_DEF_TEL_LEN+1];
    char m_pswd[33];
    char m_name[_DEF_USER_LEN*2];
};
struct User{
	int ID;
};
struct UserInfo
{
    int icon;
    char m_name[_DEF_USER_LEN*2];
    char m_sign[_DEF_SIGN_LEN*2];
    int id;
};
struct JoinZone{
    int m_playerid;
    int m_zoneid;
};
struct FiveNode_RoomInfo{
    int m_roomid;
    int m_icons[2];
    //char m_names[2][_DEF_USER_LEN*2];
};
struct JoinRoom{
    int m_roomid;
};
struct GameRoomInfo{
    int m_icons[2];
    char m_name[2][_DEF_USER_LEN*2];
    int m_color;
};
struct PieceDown{

    int m_x;
    int m_y;
    int m_color;
    int m_roomid;
    int m_zoneid;
};

struct Win{
    int m_color;
};

//回复用
struct Reply{
	int m_type;
	int m_res;
	union {
		ShowMsg m_msg;
		TimeRe m_time;
		User m_user;
        	UserInfo m_userinfo;
		FiveNode_RoomInfo m_roominfo;
		JoinZone m_zoneinfo;
		JoinRoom m_roomid;
        GameRoomInfo m_gameroominfo;
        PieceDown m_piecedown;
        Win m_win;
    }data;

};
//发送用
struct Msg_Show{
    int m_type;
    ShowMsg m_msg;
};

struct Msg_Time{
    int m_type;
};

struct Msg_Login{
    int m_type;
    Login m_login;
};

struct Msg_Register{
    int m_type;
    Register m_reg;
};
struct Msg_JoinZone{
    int m_type;
    JoinZone m_zone;
};
struct Msg_LeaveZone{
    int m_type;
    JoinZone m_zone;
};

struct Msg_CreateRoom{
    int m_type;
    int m_userid;
    int m_zoneid;
};

struct Msg_JoinRoom{
    int m_type;
    int m_roomid;
    int m_userid;
    int m_zoneid;
};

struct Msg_LeaveRoom{
    int m_type;
    int m_roomid;
    int m_userid;
    int m_zoneid;
};
struct Msg_RefreshRooms{
    int m_type;
    int m_zoneid;
};

struct Msg_PieceDown{
    int m_type;
    PieceDown piecedown;
};
struct Msg_Win{
    int m_type;
    int m_color;
    int m_roomid;
	int m_zoneid;
};

struct Msg_WinAct{
    int m_type;
    int m_act;
	int m_color;
    int m_roomid;
	int m_zoneid;
};

struct Msg_StartGame{
  int m_type;
   int m_roomid;
   int m_zoneid;
};

struct DataDeal{
	int m_type;
	union {
//		Msg m_msg;
	}data;
};
//kernel处理用
struct DealMsg{
	char* data;
	int m_sFrom;
};


#endif
