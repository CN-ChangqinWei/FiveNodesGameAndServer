#include "gameroom.h"
#include "ui_gameroom.h"
#include"./Mediator/TCPMediator.h"
#include"protocl.h"
#include"Kernel.h"
#include"QCloseEvent"
#include"QDebug"
GameRoom::GameRoom(CTCPMediator* _med) :

    ui(new Ui::GameRoom)
{
    ui->setupUi(this);
    m_pMed=_med;
    m_pMed->room_Ptrset(this);
    m_pGame=(FiveNodesWidget*)ui->WG_Game;
    m_pMed->game_ptrset(m_pGame);
    m_timer.setInterval(1000);
    m_timer.stop();

    m_counter=_DEF_TIMERELOAD;
    ui->LB_Count->hide();
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(slot_count()));
}

void GameRoom::closeEvent(QCloseEvent *event){
    m_pMed->show_gamezone();
    Msg_LeaveRoom* msg=new Msg_LeaveRoom;
    m_pGame->restart();
    m_pGame->slot_close();
    m_timer.stop();
    ui->LB_Count->hide();
    msg->m_type=_DEF_TCP_LEAVEROOM;

    msg->m_roomid=m_RoomID;
    msg->m_zoneid=0;
    msg->m_userid=CKernel::ker->get_userid();
    m_pMed->send((char*)msg,sizeof(Msg_LeaveRoom),0);
    hide();
    Sleep(100);
    event->accept();
}

void GameRoom::refresh(GameRoomInfo info){
    qDebug()<<info.m_color;
    m_pGame->slot_set_color((Pieces)info.m_color);
    m_pGame->slot_set_AIcolor((Pieces)info.m_color);
    cout<<"refresh"<<endl;
    int icon0=info.m_icons[0];
    int icon1=info.m_icons[1];
    if(icon1<0) m_pGame->slot_close();
    const char* name0=info.m_name[0];
    const char* name1=info.m_name[1];
    if(!m_pGame->isRun()) ui->LB_WHOSETURN->setText(QString("未开始"));
    if(black==(Pieces)m_pGame->get_color()){

        ui->LB_Host0->setText((QString("房主")));
        ui->LB_Host1->setText(QString(""));
        ui->LB_MINECOLOR->setText(QString("棋子颜色:黑色"));
        if(icon1<0){
            ui->LB_ENEMYCOLOR->setText(QString(""));
            ui->PB_Start->setDisabled(true);
            m_timer.stop();;
            m_counter=_DEF_TIMERELOAD;
            m_pGame->slot_close();
            m_pGame->slot_close_AI();
        }
        else{

            ui->LB_ENEMYCOLOR->setText(QString("棋子颜色:白色"));
            ui->PB_Start->setDisabled(false);
        }
    }
    else{
        ui->PB_Start->setDisabled(true);
        ui->LB_Host0->setText((QString("")));
        ui->LB_Host1->setText(QString("房主"));
        ui->LB_MINECOLOR->setText(QString("棋子颜色:白色"));
        ui->LB_ENEMYCOLOR->setText(QString("棋子颜色:黑色"));
    }
    if(icon0<0){
        ui->PB_Icon0->setIcon(QIcon(":/logo/未知人员.jpg"));
        ui->PB_Icon0->setIconSize(ui->PB_Icon0->size());
        m_exist[0]=false;
        ui->LB_Name0->setText(QString(""));
        ui->LB_Host0->setText(QString(""));
    }else{
        ui->PB_Icon0->setIcon(QIcon(QString(":/头像/btn_avatar_a%1.png").arg(icon0)));
        ui->PB_Icon0->setIconSize(ui->PB_Icon0->size());
        m_exist[0]=true;
        ui->LB_Name0->setText(QString(name0));

    }
    if(icon1<0){
        ui->PB_Icon1->setIcon(QIcon(":/logo/未知人员.jpg"));
        ui->PB_Icon1->setIconSize(ui->PB_Icon1->size());
        m_exist[1]=false;
        ui->LB_Name1->setText(QString(""));
        ui->LB_Host1->setText(QString(""));
    }else{
        ui->PB_Icon1->setIcon(QIcon(QString(":/头像/btn_avatar_a%1.png").arg(icon1)));
        ui->PB_Icon1->setIconSize(ui->PB_Icon1->size());
        m_exist[1]=true;
        ui->LB_Name1->setText(QString(name1));

    }

}

void GameRoom::slot_openroom(int roomid){
    m_RoomID=roomid;
}

void GameRoom::slot_refresh(GameRoomInfo info){
    refresh(info);
}
int GameRoom::get_roomid(){
    return m_RoomID;
}
GameRoom::~GameRoom()
{
    delete ui;
}

void GameRoom::on_PB_AImode_clicked()
{
    m_pGame->slot_CHAIStat();
}

void GameRoom::on_pushButton_clicked()
{

}

void GameRoom::on_PB_Start_clicked()
{
    if(black==m_pGame->get_color()){
        m_pGame->restart();

            qDebug()<<"start";

            m_pGame->slot_close_AI();
            //m_pGame->slot_start();
            Msg_StartGame* msg=new Msg_StartGame;
            msg->m_type=_DEF_TCP_STARTGAME;
            msg->m_roomid=m_RoomID;
            msg->m_zoneid=0;
            m_pMed->send((char*)msg,sizeof(Msg_StartGame),0);


    }
    //

}

void GameRoom::slot_gamewin(Pieces color){
    QString str;
    m_timer.stop();
    if(black==color) str=QString("黑方胜利");
    else if(white==color) str=QString("白方胜利");
    else str=QString("平局");
    ui->LB_WHOSETURN->setText(str);
    m_pGame->slot_close_AI();
    m_pGame->slot_close();
    m_timer.stop();
    ui->PB_Start->setDisabled(false);
    ui->LB_Count->hide();
}
void GameRoom::slot_piecedown(QPoint point,Pieces color){

    m_pGame->slot_setdownpieces(point,color);
    m_counter=_DEF_TIMERELOAD;
    if(black==m_pGame->get_turn())
    ui->LB_WHOSETURN->setText(QString("黑方回合"));
    else  ui->LB_WHOSETURN->setText(QString("白方回合"));
}
void GameRoom::slot_startgame(){
    qDebug()<<__func__;
    ui->LB_Count->show();
    ui->PB_Start->setDisabled(true);
    m_pGame->restart();
    m_pGame->slot_start();
    m_timer.start();
    m_counter=_DEF_TIMERELOAD;
    if(black==m_pGame->get_turn())
    ui->LB_WHOSETURN->setText(QString("黑方回合"));
    else  ui->LB_WHOSETURN->setText(QString("白方回合"));
}
void GameRoom::slot_chturn(){
    m_counter=_DEF_TIMERELOAD;
    if(black==m_pGame->get_turn())
    ui->LB_WHOSETURN->setText(QString("黑方回合"));
    else  ui->LB_WHOSETURN->setText(QString("白方回合"));
}
void GameRoom::ch_gameturn(){
    m_counter=_DEF_TIMERELOAD;
    m_pGame->ch_turn();
    slot_chturn();

}
void GameRoom::slot_count(){
    if(black==m_pGame->get_turn())
    ui->LB_WHOSETURN->setText(QString("黑方回合"));
    else  ui->LB_WHOSETURN->setText(QString("白方回合"));
    ui->LB_Count->setText(QString("当前回合结束还有 %1秒").arg(m_counter,2,10,QChar('0')));
    if(--m_counter<=0){
        ch_gameturn();
         ui->LB_Count->setText(QString("当前回合结束还有 %1秒").arg(m_counter,2,10,QChar('0')));
         m_counter=_DEF_TIMERELOAD;
    }
}
