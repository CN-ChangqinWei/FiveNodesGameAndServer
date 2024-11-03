#include "playhall.h"
#include "ui_playhall.h"
#include "./Mediator/TCPMediator.h"
#include"hallconfig.h"
#include"QMessageBox"
#define _FUNC_PBINIT(Path)  \
{\
    GameBotton* pBotton=new GameBotton(i,Path);\
    pBotton->SetBottonColor(_DEF_BK_COLOR);\
    connect(pBotton,&GameBotton::sig_gamechange,this,&PlayHall::slot_chgame);\
    gamechose[i++]=pBotton;\
    m_pLayout->addWidget(pBotton);\
}
#define _FUNC_BKINIT(Path,Name,Ab)\
    gamebks[i]=new QPixmap(Path);\
    gamebks[i]->scaled(ui->LB_BK->size(),Qt::KeepAspectRatio);\
    gamenames[i]=Name;\
    gameabstracts[i++]=Ab;
PlayHall::PlayHall(CTCPMediator* _med) :
    ui(new Ui::PlayHall)
{
    ui->setupUi(this);
    init(_med);
}

PlayHall::~PlayHall()
{
    delete m_pFirlist;
    delete ui;
}

void PlayHall::init(CTCPMediator* _med){
    m_pMed=_med;
    m_pMed->hall_Ptrset(this);
    m_pLayout=new QHBoxLayout;
    m_pLayout->setContentsMargins(0,0,0,0);
    m_pLayout->setSpacing(3);
    ui->WG_Games->setLayout(m_pLayout);
    m_GameID=0;
    m_pFirlist=new FriendList(m_pMed);
    m_pZone=new GameZone(m_pMed);
    connect(this,&PlayHall::sig_userinfo,m_pFirlist,&FriendList::slot_userinfo);
    connect(this,&PlayHall::sig_friendinfo,m_pFirlist,&FriendList::slot_friendinfo);
    connect(this,&PlayHall::sig_zoneuserinfo,m_pZone,&GameZone::slot_userinfo);
    connect(this,&PlayHall::sig_zonestart,m_pZone,&GameZone::slot_startzone);
    pb_init();
    bk_init();

}
void PlayHall::pb_init(){
    int i=0;
//    GameBotton* m_pBotton=new GameBotton(i,_DEF_PB_FIVENODES);
//    m_pBotton->setPalette(_DEF_BK_COLOR);
    _FUNC_PBINIT(_DEF_PB_FIVENODES)
    _FUNC_PBINIT(_DEF_PB_WOLFKILL)
    _FUNC_PBINIT(_DEF_PB_PUKE)
    _FUNC_PBINIT(_DEF_PB_GAME1)
    _FUNC_PBINIT(_DEF_PB_GAME2)
}
void PlayHall::bk_init(){
    int i=0;
    _FUNC_BKINIT(_DEF_BK_FIVENODES,_DEF_NAME_FIVENODES,_DEF_AB_FIVENODES)
    _FUNC_BKINIT(_DEF_BK_WOLFKILL,_DEF_NAME_WOLFKILL,_DEF_AB_WOLFKILL)
    _FUNC_BKINIT(_DEF_BK_PUKE,_DEF_NAME_PUKE,_DEF_AB_PUKE)
    _FUNC_BKINIT(_DEF_BK_GAME1,_DEF_NAME_GAME1,_DEF_AB_GAME1)
    _FUNC_BKINIT(_DEF_BK_GAME2,_DEF_NAME_GAME2,_DEF_AB_GAME2)
    setWindowFlags(Qt::FramelessWindowHint);
    ui->WG_PlayerInfo->setPalette(_DEF_BK_COLOR);
    ui->WG_Games->setPalette(_DEF_BK_COLOR);
    ui->WG_Title->setPalette(_DEF_BK_COLOR);
    ui->SA_Games->setPalette(_DEF_BK_COLOR);
    ui->WG_Begin->setPalette(_DEF_BK_COLOR);
    ui->WG_Games->setWindowFlags(Qt::FramelessWindowHint);
    ui->SA_Games->setWindowFlags(Qt::FramelessWindowHint);
    ui->WG_Title->setWindowFlags(Qt::FramelessWindowHint);
    QPalette pe(QColor(255,255,255));
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->LB_GameTitle->setPalette(pe);
    ui->LB_GameAB->setPalette(pe);

    set_bk();

}
void PlayHall::set_bk(){
    ui->LB_BK->setPixmap(*gamebks[m_GameID]);
    ui->LB_GameTitle->setText(gamenames[m_GameID]);
    ui->LB_GameAB->setText(gameabstracts[m_GameID]);
}

void PlayHall::slot_setgameid(int id){
    if(id<0||id>=_DEF_NOFGAMES) return;
    m_GameID=id;
}


void PlayHall::slot_chgame(int id){
    m_GameID=id;
    set_bk();
}
void PlayHall::slot_userinfo(UserInfo info){
    QString path=QString(":/头像/btn_avatar_a%1.png").arg(info.icon);
    ui->PB_HeadIcon->setIcon(QIcon(path));
    ui->PB_HeadIcon->setIconSize(ui->PB_HeadIcon->size());
    ui->LB_PlayerName->setText(QString(info.m_name));
    ui->LB_PlayerSign->setText(QString(info.m_sign));
    if(strlen(info.m_sign)==0){
        ui->LB_PlayerSign->setText("这个人很懒，什么也没写~");
    }
    emit sig_userinfo(info);
    emit sig_zoneuserinfo(info);
}
void PlayHall::slot_friendinfo(UserInfo info){
    emit sig_friendinfo(info);
}

void PlayHall::on_PB_Friends_clicked()
{
    int x,y;
    x=this->x()+this->width();
    y=this->y();
    m_pFirlist->move(x,y);
    m_pMed->show_frilst();
}

void PlayHall::on_PB_StartGame_clicked()
{
    //emit sig_zonestart(gamenames[m_GameID],m_GameID);
    m_pMed->join_zone(m_GameID);

    //hide();
}

void PlayHall::on_PB_Power_clicked()
{
    hide();
    exit(0);
}
void PlayHall::slot_openzone(){
    emit sig_zonestart(gamenames[m_GameID],m_GameID);
}
