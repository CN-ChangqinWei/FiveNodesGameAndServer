#include "gamebotton.h"
#include "ui_gamebotton.h"
#include "protocl.h"


GameBotton::GameBotton(int _id,const char* _path) :
    ui(new Ui::GameBotton)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    m_GameID=_id;
    setPalette(_DEF_BK_COLOR);
    ui->PB_Game->setText(QString(""));
    ui->PB_Game->setIcon(QIcon(_path));
    ui->PB_Game->setIconSize(QSize(_DEF_PBICON_WIDTH-10,_DEF_PBICON_HEIGHT-10));
    ui->PB_Game->setFlat(true);
}

GameBotton::~GameBotton()
{
    delete ui;
}

void GameBotton::on_PB_Game_clicked()
{
    emit sig_gamechange(m_GameID);
}
void GameBotton::SetBottonColor(QPalette color){
    ui->PB_Game->setPalette(color);
}
