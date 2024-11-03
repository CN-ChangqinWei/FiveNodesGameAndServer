#pragma once
#include<QObject>
#include"NetMediator.h"
#include"../Inet/TCPClient.h"
#include<QThread>
#include"../md5/md5.h"
#include"../playhall.h"
#include"../friendlist.h"
#include"../gamezone.h"
#include"../gameroom.h"
#include"../fivenodeswidget.h"
class Dialog;
class CKernel;
class CTCPMediator:public CNetMediator{
public:
    CTCPMediator(CKernel* _ker);
    ~CTCPMediator();
    bool init();
    void close();
    int send(char* data, int len, unsigned int aimsock);
    int recv(char* buf, int len, unsigned int from);
    void deal(char* buf,int len,unsigned int from);
    void show(char* buf);
    void dia_Loginptrset(Dialog* _dia);
    void hall_Ptrset(PlayHall* _hall);
    void net_Start();
    string MD5_encrypt(const char* data);
    void login_success();
    void update_userinfo(UserInfo& info);
    void update_friendinfo(UserInfo& info);
    int get_userid();
    //Play Hall
    void show_hall();
    void join_zone(int zoneid);
    void set_gameid(int id);

    //Friend list
    void show_frilst();
    void hide_frilst();
    void frilst_Ptrset(FriendList* _fri);

    //Game zone
    void show_gamezone();
    void hide_gamezone();
    void zone_Ptrset(GameZone* _zone);
    void joinzone_success();
    void leave_gamezone(int zoneid);
    void msgbox_zone(const char* msg);
    void refreshroom(int mode,int roomid,int icon0,int icon1);
    //Game Room
    void room_Ptrset(GameRoom* _room);
    void room_join(int roomid);
    void show_room();
    void leave_room();
    void refreshgameroom(GameRoomInfo info);
    int get_roomid();

    //Game
    void game_ptrset(FiveNodesWidget* _fivenodes);
    void game_piecedown(QPoint point,Pieces color);
    void game_win(Pieces color);
    void game_start();
    void game_chturn();
    void game_timerrelaod();
private:
    CKernel* m_pKer;
    QThread* m_pThread;
    CTCPClient* m_pNet;
    Dialog* m_pLogin;
    PlayHall* m_pHall;
    FriendList* m_pFri;
    GameZone* m_pZone;
    GameRoom* m_pRoom;
    FiveNodesWidget* m_pFiveNodes;
};
