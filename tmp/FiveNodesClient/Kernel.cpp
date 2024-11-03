#include"Kernel.h"
#include<QDebug>
#define _FUNC_MAP(prot,func) m_mpfuncs[_DEF_TCP_##prot-_DEF_TCPBASE]=&CKernel::deal_##func

CKernel* CKernel::ker=nullptr;

CKernel::CKernel(){
    ker=this;
    m_pTCPMediator=new CTCPMediator(this);
    //set_config();
    m_pThread=new QThread;
    m_pDia=new Dialog;
    m_pDia->setMediator(m_pTCPMediator);
    m_pDia->show();
    m_pHall=new PlayHall(m_pTCPMediator);
    m_pHall->hide();
    mapinit();

    //moveToThread(m_pThread);
    //Sleep(1);
}
CKernel::~CKernel(){
    delete m_pTCPMediator;
}
void CKernel::slot_deal(char *buf, int len, unsigned int fromsock){
    //m_pTCPMediator->show(buf);
    //Reply* pRe=(Reply*)buf;
    int type=*(int*)buf-_DEF_TCPBASE;
    cout<<type<<endl;
    if(type<0||type>_DEF_NOF_FUNCS){
        cout<<"reply type err"<<endl;
        delete []buf;
        return;
    }
    FUNC func=m_mpfuncs[type];
    if(func){
        (this->*func)(buf,len,fromsock);
    }
    delete []buf;
}

void CKernel::mapinit(){
    _FUNC_MAP(SHOW,show);
    _FUNC_MAP(TIME,time);
    _FUNC_MAP(LOGIN,login);
    _FUNC_MAP(REGISTER,register);
    _FUNC_MAP(USER,userinfo);
    _FUNC_MAP(JOINZONE,joinzone);
    _FUNC_MAP(LEAVEZONE,leavezone);
    _FUNC_MAP(JOINROOM,joinroom);
    _FUNC_MAP(LEAVEROOM,leaveroom);
    _FUNC_MAP(CREATEROOM,createroom);
    _FUNC_MAP(REFRESHROOM,refreshroom);
    _FUNC_MAP(GAMEROOM,gameroom);
    _FUNC_MAP(PIECEDOWN,piecedown);
    _FUNC_MAP(WIN,win);
    _FUNC_MAP(STARTGAME,startgame);

}
void CKernel::dialog_replyshow(char* buf){
    m_pTCPMediator->show(buf);
}
void CKernel::deal_show(void *buf,int len,int from){
    Reply* pRe=(Reply*)buf;
    dialog_replyshow(pRe->data.m_msg.data);
}

void CKernel::deal_time(void *buf,int len,int from){
    Reply* pRe=(Reply*)buf;
    dialog_replyshow(pRe->data.m_time.content);
}
void CKernel::deal_login(void *buf,int len,int from){

    Reply* pRe=(Reply*)buf;
    int res=pRe->m_res;
    if(res){
        switch (res) {
        case _DEF_LOGIN_USERNOTEXIST:
            m_pTCPMediator->show("用户不存在");
            break;
        case _DEF_LOGIN_PSWDERR:
            m_pTCPMediator->show("密码错误");
            break;
        case _DEF_LOGIN_OTHER:
            m_pTCPMediator->show("登入其他错误");
            break;
        default:
            break;
        }

    }else{
        m_pTCPMediator->show("登入成功");
        m_userid=pRe->data.m_user.ID;
        Sleep(500);
        m_pTCPMediator->login_success();
    }
}
void CKernel::deal_register(void *buf,int len,int from){

    Reply* pRe=(Reply*)buf;
    int res=pRe->m_res;
    if(res){
        switch (res) {
        case _DEF_REGISTER_USEREXIST:
            m_pTCPMediator->show("用户已存在");
            break;
        case _DEF_REGISTER_TELUSERD:
            m_pTCPMediator->show("电话号码已注册");
            break;
        case _DEF_REGISTER_OTHRE:
            m_pTCPMediator->show("其他注册错误");
            break;
        default:
            break;
        }

    }else{
        m_pTCPMediator->show("注册成功成功");

    }

}
void CKernel::deal_joinzone(void* buf,int len,int from){
    Reply* pRe=(Reply*)buf;
    int res=pRe->m_res;

    if(_DEF_ZONE_JOINOK==res)
    {
        int id=pRe->data.m_zoneinfo.m_zoneid;
        m_pTCPMediator->set_gameid(id);
        m_pTCPMediator->joinzone_success();
    }
    else
        qDebug()<<"加入专区失败"<<endl;
}
void CKernel::deal_leavezone(void* buf,int len,int from){
    m_pTCPMediator->hide_gamezone();
    m_pTCPMediator->show_hall();
}

void CKernel::deal_createroom(void* buf,int len,int from){
    m_pTCPMediator->msgbox_zone("创建房间失败");

}
void CKernel::deal_joinroom(void* buf,int len,int from){
    Reply* re=(Reply*)buf;
    int res=re->m_res;
    int roomid=re->data.m_roomid.m_roomid;
    if(_DEF_ROOM_JOINOK==res)
    m_pTCPMediator->room_join(roomid);
    else m_pTCPMediator->msgbox_zone("加入房间失败");
}
void CKernel::deal_leaveroom(void* buf,int len,int from){
    m_pTCPMediator->leave_room();
}
void CKernel::deal_refreshroom(void* buf,int len,int from){
    Reply* re=(Reply*)buf;
    int mode=re->m_res;
    int icon0=re->data.m_roominfo.m_icons[0];
    int icon1=re->data.m_roominfo.m_icons[1];
    int roomid=re->data.m_roominfo.m_roomid;
    m_pTCPMediator->refreshroom(mode,roomid,icon0,icon1);
}

void CKernel::set_config(){
    strcpy(m_ip,_DEF_SERVERIP);
    QString path=QCoreApplication::applicationDirPath()+"/config.ini";
    QFileInfo info(path);
    if(info.exists()){
        QSettings setting(path,QSettings::IniFormat,nullptr);
        setting.beginGroup("net");
        QVariant var=setting.value("ip");
        QString strip=var.toString();
        if(!strip.isEmpty()){
            strcpy(m_ip,strip.toStdString().c_str());
        }
        setting.endGroup();
    }else{
        QSettings setting(path,QSettings::IniFormat,nullptr);
        setting.beginGroup("net");
        setting.setValue("ip",QString::fromStdString(m_ip));
        setting.endGroup();
    }

}
void CKernel::deal_userinfo(void* buf,int len,int from){
    Reply* pRe=(Reply*)buf;
    UserInfo info=pRe->data.m_userinfo;
    if(pRe->data.m_userinfo.id==m_userid){

        m_pTCPMediator->update_userinfo(info);
    }else{
        m_pTCPMediator->update_friendinfo(info);
    }
}
void CKernel::deal_gameroom(void* buf,int len,int from){
    Reply* pRe=(Reply*)buf;
    GameRoomInfo info=pRe->data.m_gameroominfo;
    m_pTCPMediator->refreshgameroom(info);

}

void CKernel::deal_piecedown(void* buf,int len,int from){
    Reply* pRe=(Reply*)buf;
    Pieces piece=(Pieces)pRe->data.m_piecedown.m_color;
    QPoint point=QPoint(pRe->data.m_piecedown.m_x,pRe->data.m_piecedown.m_y);
    m_pTCPMediator->game_piecedown(point,piece);
}
void CKernel::deal_win(void* buf,int len,int from){
    Reply* pRe=(Reply*)buf;
    Pieces color=(Pieces)pRe->data.m_win.m_color;
    m_pTCPMediator->game_win(color);
}


int CKernel::get_userid(){
    return m_userid;
}
void CKernel::slot_joinzone(int zoneid){
    Msg_JoinZone* joinzone=new Msg_JoinZone;
    joinzone->m_type=_DEF_TCP_JOINZONE;
    joinzone->m_zone.m_playerid=m_userid;
    joinzone->m_zone.m_zoneid=zoneid;
    m_pTCPMediator->send((char*)joinzone,sizeof(Msg_JoinZone),0);
}
void CKernel::slot_leavezone(int zoneid){
    Msg_LeaveZone* leavezone=new Msg_LeaveZone;
    leavezone->m_type=_DEF_TCP_LEAVEZONE;
    leavezone->m_zone.m_playerid=m_userid;
    leavezone->m_zone.m_zoneid=zoneid;
    m_pTCPMediator->send((char*)leavezone,sizeof(Msg_LeaveZone),0);
}

void CKernel::slot_piecedown(QPoint point,Pieces color){
    qDebug()<<__func__;
    Msg_PieceDown* piecedown=new Msg_PieceDown;
    piecedown->piecedown.m_color=color;
    piecedown->piecedown.m_zoneid=0;
    piecedown->piecedown.m_roomid=m_pTCPMediator->get_roomid();
    piecedown->piecedown.m_x=point.x();
    piecedown->piecedown.m_y=point.y();
    piecedown->m_type=_DEF_TCP_PIECEDOWN;
    m_pTCPMediator->game_chturn();
    m_pTCPMediator->send((char*)piecedown,sizeof(Msg_PieceDown),0);
}

void CKernel::slot_win(Pieces color){
    Msg_Win* win=new Msg_Win;
    win->m_color=color;
    win->m_roomid=m_pTCPMediator->get_roomid();
    win->m_zoneid=0;
    win->m_type=_DEF_TCP_WIN;
    m_pTCPMediator->send((char*)win,sizeof(Msg_Win),0);
}

void CKernel::deal_startgame(void* buf,int len,int from){
    m_pTCPMediator->game_start();

}
