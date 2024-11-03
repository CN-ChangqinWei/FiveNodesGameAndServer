#include "room.h"
#include "ui_room.h"
#include"./Mediator/TCPMediator.h"
#include"Kernel.h"
room::room(int id,int zoneid,CTCPMediator* _med,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::room)
{
    ui->setupUi(this);
    init(id,zoneid,_med);
}

room::~room()
{
    delete ui;
}
void room::init(int id,int zoneid,CTCPMediator *_med){
    m_pMed=_med;
    m_roomid=id;
    m_isrun=false;
    m_zoneid=zoneid;
    ui->PB_Play1HeadIcon->setIcon(QIcon(":/logo/未知人员.jpg"));
    ui->PB_Play1HeadIcon->setIconSize(ui->PB_Play1HeadIcon->size());
    ui->PB_Play2HeadIcon->setIcon(QIcon(":/logo/未知人员.jpg"));
    ui->PB_Play2HeadIcon->setIconSize(ui->PB_Play2HeadIcon->size());
    ui->LB_RoomID->setText(QString("房间号:%1").arg(m_roomid));
    ui->LB_Stat->setText(QString("等待中"));
    setAttribute(Qt::WA_StyledBackground);
}
void room::delplayer(int id){
    if(id>1||id<0) return;
    m_exist[id]=0;
    if(id==1){
        ui->PB_Play2HeadIcon->setIcon(QIcon(":/logo/未知人员.jpg"));
        ui->PB_Play2HeadIcon->setIconSize(ui->PB_Play2HeadIcon->size());
    }else{
        ui->PB_Play1HeadIcon->setIcon(QIcon(":/logo/未知人员.jpg"));
        ui->PB_Play1HeadIcon->setIconSize(ui->PB_Play1HeadIcon->size());
    }
}
void room::refresh(int icid0,int icid1){
    if(icid0<0){
        ui->PB_Play1HeadIcon->setIcon(QIcon(":/logo/未知人员.jpg"));
        ui->PB_Play1HeadIcon->setIconSize(ui->PB_Play1HeadIcon->size());
        m_exist[0]=false;
    }else{
        ui->PB_Play1HeadIcon->setIcon(QIcon(QString(":/头像/btn_avatar_a%1.png").arg(icid0)));
        ui->PB_Play1HeadIcon->setIconSize(ui->PB_Play1HeadIcon->size());
        m_exist[0]=true;
    }
    if(icid1<0){
        ui->PB_Play2HeadIcon->setIcon(QIcon(":/logo/未知人员.jpg"));
        ui->PB_Play2HeadIcon->setIconSize(ui->PB_Play2HeadIcon->size());
        m_exist[1]=false;
    }else{
        ui->PB_Play2HeadIcon->setIcon(QIcon(QString(":/头像/btn_avatar_a%1.png").arg(icid1)));
        ui->PB_Play2HeadIcon->setIconSize(ui->PB_Play2HeadIcon->size());
        m_exist[1]=true;
    }if(m_isrun){
        ui->LB_Stat->setText(QString("对战中"));
    }
    else if(icid0&&icid1){
        ui->LB_Stat->setText(QString("准备中"));
    }else{
        ui->LB_Stat->setText(QString("等待中"));
    }
}

void room::on_PB_Play1HeadIcon_clicked()
{
    if(m_exist[0]) return;
    Msg_JoinRoom* msg=new Msg_JoinRoom;
    msg->m_type=_DEF_TCP_JOINROOM;
    msg->m_roomid=m_roomid;
    msg->m_zoneid=m_zoneid;
    msg->m_userid=_DEF_KERNEL->get_userid();
    m_pMed->send((char*)msg,sizeof(Msg_JoinRoom),0);
}

void room::on_PB_Play2HeadIcon_clicked()
{
    if(m_exist[1]) return;
    Msg_JoinRoom* msg=new Msg_JoinRoom;
    msg->m_type=_DEF_TCP_JOINROOM;
    msg->m_roomid=m_roomid;
    msg->m_zoneid=m_zoneid;
    msg->m_userid=_DEF_KERNEL->get_userid();
    m_pMed->send((char*)msg,sizeof(Msg_JoinRoom),0);
}
