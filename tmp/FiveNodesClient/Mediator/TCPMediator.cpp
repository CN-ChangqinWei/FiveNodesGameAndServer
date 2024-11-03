#include"TCPMediator.h"
#include"../Kernel.h"
#include"../dialog.h"
#include"QDebug"
CTCPMediator::CTCPMediator(CKernel* _ker){

    m_pKer= _ker;
    m_pNet=new CTCPClient(this);
    init();
    //emit sig_netstart();
    Sleep(1);
//    m_pThread=new QThread;
//    moveToThread(m_pThread);
}
CTCPMediator::~CTCPMediator(){
    delete m_pNet;
}
bool CTCPMediator:: init(){
    connect(this,&CTCPMediator::sig_send,m_pNet,&CTCPClient::slot_send);
    connect(this,&CTCPMediator::sig_recv,m_pNet,&CTCPClient::slot_recv);
    connect(this,&CTCPMediator::sig_deal,m_pKer,&CKernel::slot_deal);
    connect(this,&CTCPMediator::sig_netstart,m_pNet,&CTCPClient::slot_start);
    connect(this,&CTCPMediator::sig_joinzone,m_pKer,&CKernel::slot_joinzone);
    connect(this,&CTCPMediator::sig_leavezone,m_pKer,&CKernel::slot_leavezone);
    return false;
}
void CTCPMediator::close(){

}
int CTCPMediator::send(char* data, int len, unsigned int aimsock){

    emit sig_send(data,len,aimsock);
}
int CTCPMediator::recv(char* buf, int len, unsigned int from){
    emit sig_recv(buf,len,from);
}
void CTCPMediator::deal(char* buf,int len,unsigned int from){
    emit sig_deal(buf,len,from);
}
void CTCPMediator::show(char* buf){
    //emit sig_show(buf);
    m_pLogin->showReply(buf);
}
void CTCPMediator::dia_Loginptrset(Dialog* _dia){
    m_pLogin=_dia;
    connect(this,&CTCPMediator::sig_show,m_pLogin,&Dialog::slot_show);
    connect(this,&CTCPMediator::sig_hidendia,m_pLogin,&Dialog::hide);
}
void CTCPMediator::hall_Ptrset(PlayHall* _hall){
    m_pHall=_hall;
    connect(this,&CTCPMediator::sig_showhall,m_pHall,&PlayHall::show);
    connect(this,&CTCPMediator::sig_hidenhall,m_pHall,&PlayHall::hide);
    connect(this,&CTCPMediator::sig_userinfo,m_pHall,&PlayHall::slot_userinfo);
    connect(this,&CTCPMediator::sig_friendinfo,m_pHall,&PlayHall::slot_friendinfo);
    connect(this,&CTCPMediator::sig_joinzonesuccess,m_pHall,&PlayHall::slot_openzone);
    connect(this,&CTCPMediator::sig_setgameid,m_pHall,&PlayHall::slot_setgameid);
}

void CTCPMediator::frilst_Ptrset(FriendList* _fri){
    m_pFri=_fri;
    connect(this,&CTCPMediator::sig_frilsthide,m_pFri,&FriendList::hide);
    connect(this,&CTCPMediator::sig_frilstshow,m_pFri,&FriendList::show);
}
void CTCPMediator::zone_Ptrset(GameZone* _zone){
    m_pZone=_zone;
    connect(this,&CTCPMediator::sig_gamezonehide,m_pZone,&GameZone::slot_close);
    connect(this,&CTCPMediator::sig_gamezoneshow,m_pZone,&GameZone::slot_open);
    connect(this,&CTCPMediator::sig_msgbox_zone,m_pZone,&GameZone::slot_msgbox);
    connect(this,&CTCPMediator::sig_refreshroom,m_pZone,&GameZone::slot_refreshroom);
}
void CTCPMediator::room_Ptrset(GameRoom* _room){
    m_pRoom=_room;
    connect(this,&CTCPMediator::sig_showroom,m_pRoom,&GameRoom::show);
    connect(this,&CTCPMediator::sig_hideroom,m_pRoom,&GameRoom::hide);
    connect(this,&CTCPMediator::sig_joinroomsuccess,m_pRoom,&GameRoom::slot_openroom);
    connect(this,&CTCPMediator::sig_refreshgameroom,m_pRoom,&GameRoom::slot_refresh);
    connect(this,&CTCPMediator::sig_piecedown,m_pRoom,&GameRoom::slot_piecedown);
    connect(this,&CTCPMediator::sig_startgame,m_pRoom,&GameRoom::slot_startgame);
    connect(this,&CTCPMediator::sig_gamewin,m_pRoom,&GameRoom::slot_gamewin);
    connect(this,&CTCPMediator::sig_chturn,m_pRoom,&GameRoom::slot_chturn);
}
void CTCPMediator::game_ptrset(FiveNodesWidget* _fivenodes){
    m_pFiveNodes=_fivenodes;

    connect(m_pFiveNodes,&FiveNodesWidget::sig_pciecedown,m_pKer,&CKernel::slot_piecedown);
    connect(m_pFiveNodes,&FiveNodesWidget::sig_playerwin,m_pKer,&CKernel::slot_win);


}

void CTCPMediator::refreshgameroom(GameRoomInfo info){
    emit sig_refreshgameroom(info);

}
void CTCPMediator::net_Start(){
    emit sig_netstart();
}
string CTCPMediator::MD5_encrypt(const char* data){
    MD5 md5(data);
    return  md5.toString();
}
void CTCPMediator::login_success(){
    emit sig_hidendia();
    emit sig_showhall();
}

void CTCPMediator::show_hall(){
    emit sig_showhall();
}

void CTCPMediator::update_userinfo(UserInfo& info){
    emit sig_userinfo(info);
}
void CTCPMediator::update_friendinfo(UserInfo& info){
    emit sig_friendinfo(info);
}
//friends lst;
void CTCPMediator::show_frilst(){
    emit sig_frilstshow();
}
void CTCPMediator::hide_frilst(){
    emit sig_frilsthide();
}

void CTCPMediator::show_gamezone(){
    emit sig_gamezoneshow();
}
void CTCPMediator::hide_gamezone(){
    emit sig_gamezonehide();
}
void CTCPMediator::join_zone(int zoneid){
    emit sig_joinzone(zoneid);
}
void CTCPMediator::joinzone_success(){
    emit sig_joinzonesuccess();
    emit sig_hidenhall();
}
void CTCPMediator::set_gameid(int id){
    emit sig_setgameid(id);

}
void CTCPMediator::leave_gamezone(int zoneid){
    emit sig_leavezone(zoneid);

}
void CTCPMediator::msgbox_zone(const char* msg){
    emit sig_msgbox_zone(msg);
}
void CTCPMediator::room_join(int roomid){
    emit sig_joinroomsuccess(roomid);
    emit sig_showroom();
    emit sig_gamezonehide();
}
void CTCPMediator::leave_room(){
    emit sig_gamezoneshow();
    emit sig_hideroom();
}
void CTCPMediator::refreshroom(int mode,int roomid,int icon0,int icon1){
    emit sig_refreshroom(mode,roomid,icon0,icon1);
}
int CTCPMediator::get_userid(){
    return m_pKer->get_userid();

}
int CTCPMediator::get_roomid(){
    return m_pRoom->get_roomid();

}
void CTCPMediator::game_piecedown(QPoint point,Pieces color){
    emit sig_piecedown(point,color);

}
void CTCPMediator::game_start(){
    qDebug()<<__func__;
    emit sig_startgame();

}
void CTCPMediator::game_win(Pieces color){
    emit sig_gamewin(color);

}
void CTCPMediator::game_chturn(){
    m_pRoom->slot_chturn();

}
