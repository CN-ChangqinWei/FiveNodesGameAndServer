#ifndef PLAYHALL_H
#define PLAYHALL_H

#include <QDialog>
#include<QLayout>
#include"protocl.h"
#include"gamebotton.h"
#include"hallconfig.h"
#include"friendlist.h"
#include"gamezone.h"
class CTCPMediator;
namespace Ui {
class PlayHall;
}

class PlayHall : public QDialog
{
    Q_OBJECT
signals:
    void sig_userinfo(UserInfo info);
    void sig_friendinfo(UserInfo info);
    void sig_zoneuserinfo(UserInfo info);
    void sig_zonestart(const char* game,int gameid);
public:
    explicit PlayHall(CTCPMediator* _med);
    ~PlayHall();
    void init(CTCPMediator* _med);
    void set_bk();

public slots:
    void slot_chgame(int id);
    void slot_userinfo(UserInfo info);
    void slot_friendinfo(UserInfo info);
    void slot_openzone();
    void slot_setgameid(int id);
private slots:
    void on_PB_Friends_clicked();

    void on_PB_StartGame_clicked();

    void on_PB_Power_clicked();

private:
    void pb_init();
    void bk_init();
private:
    Ui::PlayHall *ui;
    FriendList* m_pFirlist;
    CTCPMediator* m_pMed;
    QHBoxLayout* m_pLayout;
    QPixmap* gamebks[_DEF_NOFGAMES];
    GameBotton* gamechose[_DEF_NOFGAMES];
    GameZone* m_pZone;
    const char* gamenames[_DEF_NOFGAMES];
    const char* gameabstracts[_DEF_NOFGAMES];
    int m_GameID;
};

#endif // PLAYHALL_H
