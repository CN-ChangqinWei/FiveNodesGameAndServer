#ifndef GAMEZONE_H
#define GAMEZONE_H

#include <QDialog>
#include"protocl.h"
#include"room.h"
#include<list>
#include<QMessageBox>
#include<ctime>
#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include"gameroom.h"
#define _DEF_ZONE   (GameZone::zone)
using namespace std;
class CTCPMediator;
namespace Ui {
class GameZone;
}

class GameZone : public QDialog
{
    Q_OBJECT

public:
    explicit GameZone(CTCPMediator* _med);
    ~GameZone();
    void init(CTCPMediator* _med);
    void closeEvent(QCloseEvent* event);
    void addRooms(int roomid,int icon0,int icon1);
    void delRooms(int roomid);
    void refreshRooms();
    void req_refreshrooms();
    void clearroom();
    bool isRun();
    void get_roomslist();

    static void __stdcall RoomRefresh ( HWND hwnd, unsigned message, unsigned iTimerID, unsigned long dwTime);
    static GameZone* zone;
public slots:
    void slot_userinfo(UserInfo info);
    void slot_startzone(const char* game,int gameid);
    void slot_msgbox(const char* msg);
    void slot_joinroom(int roomid);
    void slot_refreshroom(int mode,int roomid,int icon0,int icon1);
    void slot_showgamezone();
    void slot_close();
    void slot_open();
private slots:
    void on_pushButton_clicked();

    void on_PB_CreateRoom_clicked();

    void on_PB_FastMatch_clicked();

private:
    Ui::GameZone *ui;
    CTCPMediator* m_pMed;
    int m_GameID;
    bool m_isRun;
    QLayout* m_pLayout;
    map<int,room*> m_mpRooms;
    list<QWidget*> m_lstRoomContain;
    QWidget* pTemp;
    static HWND m_hWND;
    GameRoom* m_GameRoom;
};

#endif // GAMEZONE_H
