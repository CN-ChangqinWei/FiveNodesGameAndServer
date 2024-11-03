#include "friendlist.h"
#include "ui_friendlist.h"
#include "./Mediator/TCPMediator.h"
FriendList::FriendList(CTCPMediator* _med) :

    ui(new Ui::FriendList)
{
    ui->setupUi(this);
    init(_med);
}

FriendList::~FriendList()
{
    delete ui;
}
void FriendList::init(CTCPMediator* _med){
    m_pMed=_med;
    m_pMed->frilst_Ptrset(this);
}
void FriendList::closeEvent(QCloseEvent* _event){
    hide();
}

void FriendList::slot_userinfo(UserInfo info){
    QString path=QString(":/头像/btn_avatar_a%1.png").arg(info.icon);
    ui->PB_HeadIcon->setIcon(QIcon(path));
    ui->PB_HeadIcon->setIconSize(ui->PB_HeadIcon->size());
    ui->LB_Name->setText(QString(info.m_name));
    ui->LB_Sign->setText(QString(info.m_sign));
    if(strlen(info.m_sign)==0){
        ui->LB_Sign->setText("这个人很懒，什么也没写~");
    }
}

void FriendList::slot_friendinfo(UserInfo info){

}
