#include"./Mediator/TCPMediator.h"
#include"QObject"
#include"QThread"
#include"dialog.h"
#include"protocl.h"
#include<QSettings>
#include<QCoreApplication>
#include<QFileInfo>
#include<map>
#include"playhall.h"
#include<ctime>
#define _DEF_KERNEL (CKernel::ker)
class CKernel:public QObject{
    Q_OBJECT
public:
    typedef void (CKernel::*FUNC)(void *buf,int len,int from);
    CKernel();
    ~CKernel();
    void mapinit();
    void dialog_replyshow(char* buf);
    //map funcs
    void deal_show(void *buf,int len,int from);
    void deal_time(void *buf,int len,int from);
    void deal_login(void *buf,int len,int from);
    void deal_register(void *buf,int len,int from);
    void deal_userinfo(void* buf,int len,int from);
    void deal_joinzone(void* buf,int len,int from);
    void deal_leavezone(void* buf,int len,int from);
    void deal_createroom(void* buf,int len,int from);
    void deal_joinroom(void* buf,int len,int from);
    void deal_leaveroom(void* buf,int len,int from);
    void deal_refreshroom(void* buf,int len,int from);
    void deal_gameroom(void* buf,int len,int from);
    void deal_piecedown(void* buf,int len,int from);
    void deal_win(void* buf,int len,int from);
    void deal_startgame(void* buf,int len,int from);
    //void deal_winact(void* buf,int len,int from);
    void set_config();
    int get_userid();
    static CKernel* ker;
public slots:
    void slot_deal(char* buf,int len,unsigned int fromsock);
    void slot_joinzone(int zoneid);
    void slot_leavezone(int zoneid);

    void slot_piecedown(QPoint point,Pieces color);
    void slot_win(Pieces color);
private:
    CTCPMediator* m_pTCPMediator;
    QThread* m_pThread;
    Dialog* m_pDia;
    PlayHall* m_pHall;
    map<int,FUNC> m_mpfuncs;
    char m_ip[20];
    int m_userid;
};
