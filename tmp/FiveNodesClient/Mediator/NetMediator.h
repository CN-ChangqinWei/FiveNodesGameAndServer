#pragma once
#include"../Inet/Inet.h"
#include"../protocl.h"
#include"../fivenodeswidget.h"
class CNetMediator:public QObject{
    Q_OBJECT
public:
    CNetMediator():QObject(nullptr){}
    virtual ~CNetMediator(){}
    virtual bool init()=0;
    virtual void close()=0;
    virtual int send(char* data, int len, unsigned int aimsock)=0;
    virtual int recv(char* buf, int len, unsigned int from)=0;

signals:
    void sig_send(char *data, int len, unsigned int aimsock);
    void sig_recv(char *buf, int len, unsigned int from);
    void sig_deal(char *buf,int len,unsigned int from);
    void sig_show(char *buf);
    void sig_netstart();
    void sig_netclose();
    void sig_hidendia();
    void sig_showhall();
    void sig_hidenhall();

    void sig_setgameid(int id);
    void sig_userinfo(UserInfo info);
    void sig_friendinfo(UserInfo info);
    //Friend list
    void sig_frilstshow();
    void sig_frilsthide();
    //Game zone
    void sig_gamezoneshow();
    void sig_gamezonehide();
    void sig_joinzone(int zoneid);
    void sig_joinzonesuccess();
    void sig_leavezone(int zoneid);
    void sig_msgbox_zone(const char* msg);
    void sig_joinroomsuccess(int roomid);
    void sig_refreshroom(int mode,int roomid,int icon0,int icon1);

    //Game Room
    void sig_showroom();
    void sig_hideroom();
    void sig_refreshgameroom(GameRoomInfo info);
    void sig_piecedown(QPoint,Pieces );
    void sig_startgame();
    void sig_gamewin(Pieces color);
    void sig_chturn();
};

