#include "gamezone.h"
#include "ui_gamezone.h"
#include "./Mediator/TCPMediator.h"
#include "hallconfig.h"
HWND GameZone::m_hWND=0;
GameZone::GameZone(CTCPMediator* _med):
    ui(new Ui::GameZone)
{
    ui->setupUi(this);
    init(_med);
}

GameZone::~GameZone()
{
    if(pTemp) delete pTemp;
    delete ui;
}

void GameZone::init(CTCPMediator* _med){
    zone=this;
    m_GameRoom=new GameRoom(_med);
    m_isRun=false;
    m_hWND=(HWND)this->winId();
    m_pMed=_med;
    m_pMed->zone_Ptrset(this);
    m_pLayout=new QVBoxLayout;
    m_pLayout->setContentsMargins(0,0,0,0);
    m_pLayout->setSpacing(3);
    ui->WG_RealTimeRooms->setLayout(m_pLayout);
    pTemp=nullptr;
    srand(time(NULL));
//    for(int i=0;i<10;i++){
//        addRooms(i*3,10+i,20+i);
//        addRooms(i*3+1,-1,20+i);
//        addRooms(i*3+2,10+i,-1);
//    }
    refreshRooms();
}
void GameZone::slot_userinfo(UserInfo info){
    QString path=QString(":/头像/btn_avatar_a%1.png").arg(info.icon);
    ui->PB_HeadIcon->setIcon(QIcon(path));
    ui->PB_HeadIcon->setIconSize(ui->PB_HeadIcon->size());
    ui->LB_Name->setText(QString(info.m_name));
    ui->LB_Sign->setText(QString(info.m_sign));
    if(strlen(info.m_sign)==0){
        ui->LB_Sign->setText("这个人很懒，什么也没写~");
    }

}
void GameZone::slot_startzone(const char* game,int gameid){
    m_isRun=true;
    ui->LB_GameName->setText(game);
    SetTimer(m_hWND,0,2000,GameZone::RoomRefresh);
    m_GameID=gameid;
    show();
}

void GameZone::slot_msgbox(const char* msg){

    QMessageBox::about(this,QString("消息"),QString(msg));

}
void GameZone::slot_joinroom(int roomid){


}

void GameZone::slot_refreshroom(int mode,int roomid,int icon0,int icon1){
    if(_DEF_ROOMREFRESH_BEGIN==mode){
        cout<<"refresh begin"<<endl;
        clearroom();

    }else if(_DEF_ROOMREFRESH_TRANS==mode){
        addRooms(roomid,icon0,icon1);
    }else{
        cout<<"refresh end"<<endl;
        refreshRooms();
    }
}
void GameZone::slot_showgamezone(){
    clearroom();
    m_isRun=true;

}
void GameZone::closeEvent(QCloseEvent* event){
    m_isRun=false;
    m_pMed->leave_gamezone(m_GameID);
    Sleep(1000);
}
void GameZone::addRooms(int roomid,int icon0,int icon1){
    if(m_mpRooms.find(roomid)!=m_mpRooms.end()) return;
    if(roomid>=_DEF_MAXROOMS) return;
    room* pRoom=new room(roomid,m_GameID,m_pMed);
    pRoom->refresh(icon0,icon1);
    m_mpRooms[roomid]=pRoom;
    //m_pLayout->addWidget(pRoom);
    //m_Roomlst.push_back(pRoom);

}
void GameZone::delRooms(int roomid){
    if(m_mpRooms.find(roomid)!=m_mpRooms.end()){
        m_mpRooms[roomid]->setParent(nullptr);
        delete m_mpRooms[roomid];
        m_mpRooms.erase(m_mpRooms.find(roomid));
    }
}
void GameZone::refreshRooms(){
    for(int i=0;i<_DEF_MAXROOMS;i++){
        if(m_mpRooms.find(i)!=m_mpRooms.end()){

            m_mpRooms[i]->setParent(nullptr);
        }
    }
    for(auto v:m_lstRoomContain) delete v;
    m_lstRoomContain.clear();
    int count=0;

    for(int i=0;i<_DEF_MAXROOMS;i++){
        if(m_mpRooms.find(i)!=m_mpRooms.end()){
            if(count%2){
                pTemp->setParent(nullptr);
                m_lstRoomContain.back()->layout()->removeWidget(pTemp);
                pTemp=nullptr;
                //delete pTemp;
                m_lstRoomContain.back()->layout()->addWidget(m_mpRooms[i]);
            }else{
                pTemp=new QWidget;
                pTemp->setMinimumSize(_DEF_WIDTH_ROOM,_DEF_HEIGHT_ROOM);
                pTemp->setMaximumSize(_DEF_WIDTH_ROOM,_DEF_HEIGHT_ROOM);
                pTemp->setPalette(QPalette(QColor(0,0,0,0)));
                  QWidget* pWidget=new QWidget;
                  QLayout* pLayout=new QHBoxLayout;
                  pLayout->setContentsMargins(0,0,0,0);
                  pLayout->setSpacing(0);
                  pWidget->setMinimumSize(2*_DEF_WIDTH_ROOM,_DEF_HEIGHT_ROOM);
                  pWidget->setMaximumSize(2*_DEF_WIDTH_ROOM,_DEF_HEIGHT_ROOM);
                  pWidget->setPalette(QPalette(QColor(0,0,0,0)));
                  pWidget->setLayout(pLayout);
                  pLayout->addWidget(m_mpRooms[i]);
                  pLayout->addWidget(pTemp);
                  m_pLayout->addWidget(pWidget);
                  m_lstRoomContain.push_back(pWidget);
            }
            count++;
        }
    }
}



void GameZone::on_PB_CreateRoom_clicked()
{
    Msg_CreateRoom* msg=new Msg_CreateRoom;
    msg->m_type=_DEF_TCP_CREATEROOM;
    msg->m_userid=m_pMed->get_userid();
    msg->m_zoneid=m_GameID;
    m_pMed->send((char*)msg,sizeof(Msg_CreateRoom),0);
    //addRooms(rand()%_DEF_MAXROOMS,-1,-1);
    //refreshRooms();
}

void GameZone::on_PB_FastMatch_clicked()
{
//    delRooms(rand()%_DEF_MAXROOMS);
    clearroom();
    refreshRooms();
}
void GameZone::clearroom(){
    for(pair<int,room*> v:m_mpRooms){
        v.second->setParent(nullptr);
        delete v.second;
        m_mpRooms.erase(m_mpRooms.find(v.first));
    }
}
void GameZone::on_pushButton_clicked(){


}
void  __stdcall GameZone:: RoomRefresh ( HWND hwnd, unsigned message, unsigned iTimerID, unsigned long dwTime){
    KillTimer(GameZone::m_hWND,0);
    if(_DEF_ZONE->isRun()){
        _DEF_ZONE->get_roomslist();
        SetTimer(hwnd,0,2000,GameZone::RoomRefresh);
    }

}
bool GameZone::isRun(){
    return m_isRun;
}
void GameZone::get_roomslist(){
    Msg_RefreshRooms* requi=new Msg_RefreshRooms;
    requi->m_type=_DEF_TCP_REFRESHROOM;
    requi->m_zoneid=m_GameID;
    m_pMed->send((char*)requi,sizeof(Msg_RefreshRooms),0);
}

void GameZone::req_refreshrooms(){


    //m_pMed->send

}
void GameZone::slot_close(){
    m_isRun=false;
    hide();
}
void GameZone::slot_open(){
    SetTimer(m_hWND,0,2000,GameZone::RoomRefresh);
    m_isRun=true;
    show();

}
GameZone* GameZone::zone=nullptr;
