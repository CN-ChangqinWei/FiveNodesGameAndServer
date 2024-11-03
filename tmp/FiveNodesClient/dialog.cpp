#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::slot_show(char* buf){
    cout<<__func__<<endl;
    if(nullptr==buf) return;
    ui->LB_ReplyShow->clear();
    ui->LB_ReplyShow->setText(QString(buf));
    //delete []buf;

}
void Dialog::setMediator(CTCPMediator* _med){
    if(nullptr==_med) return;
    m_pMed=_med;
    m_pMed->dia_Loginptrset(this);
}



void Dialog::on_PB_GetTime_clicked()
{
    Msg_Time* msg=(Msg_Time*)malloc(sizeof(Msg_Time));
    msg->m_type=_DEF_TCP_TIME;
    m_pMed->send((char*)msg,sizeof(Msg_Time),0);
}

void Dialog::on_PB_LoginClear_clicked()
{
    ui->LE_LoginPassword->setText(QString(""));
    ui->LE_LoginUserName->setText(QString(""));
}

void Dialog::on_PB_LoginSubit_clicked()
{


    QString tel=ui->LE_LoginUserName->text();
    QString pswd=ui->LE_LoginPassword->text();

    //tel
    //长度检测
    if(tel.size()!=_DEF_TEL_LEN){
        ui->LB_ReplyShow->setText("电话号码长度错误");
        return;
    }
    //非法字符检测
    for(QChar v:tel){
        if(!(v>='0'&&v<='9'))
        {
            ui->LB_ReplyShow->setText("电话号码有非法字符");
            return;
        }
    }
    //pswd
    //长度检测
    if(pswd.size()<6){
        ui->LB_ReplyShow->setText("密码太短小于六位");
        return;
    }
    if(pswd.size()>_DEF_PSWD_LEN){
        ui->LB_ReplyShow->setText("密码太长");
        return;
    }
    //非法字符检测
    for(QChar v:pswd){
        if('!'==v||'#'==v||'$'==v||'%'==v||'^'==v||'&'==v||'*'==v||'('==v||')'==v||'-'==v||' '==v)
        {
            ui->LB_ReplyShow->setText("密码有非法字符");
            return;
        }
    }
    Msg_Login* log=new Msg_Login;
    log->m_type=_DEF_TCP_LOGIN;
     pswd=QString("%1_MD5KEY").arg(pswd);
    strcpy(log->m_login.m_tel,tel.toStdString().c_str());
    strcpy(log->m_login.m_pswd,m_pMed->MD5_encrypt(pswd.toStdString().c_str()).c_str());
    m_pMed->send((char*)log,sizeof(Msg_Login),0);
}

void Dialog::on_PB_RegisterClear_clicked()
{
    ui->LE_RegisterPassword->setText(QString(""));
    ui->LE_RegisterUserName->setText(QString(""));
    ui->LE_RegisterEnsure->setText(QString(""));
    ui->LE_RegisterTel->setText(QString(""));
}

void Dialog::on_PB_RegisterSubit_clicked()
{
    QString name=ui->LE_RegisterUserName->text();
    QString pswd=ui->LE_RegisterPassword->text();
    QString ensure=ui->LE_RegisterEnsure->text();
    QString tel=ui->LE_RegisterTel->text();
    //name
    //长度
    if(name.size()<6){
        ui->LB_ReplyShow->setText("用户名太短小于六位");
        return;
    }
    if(name.size()>_DEF_USER_LEN){
        ui->LB_ReplyShow->setText("用户名太长");
        return;
    }
    //非法字符检测
    for(QChar v:pswd){
        if('!'==v||'#'==v||'$'==v||'%'==v||'^'==v||'&'==v||'*'==v||'('==v||')'==v||'-'==v||' '==v)
        {
            ui->LB_ReplyShow->setText("用户名有非法字符");
            return;
        }
    }

    //tel
    //长度检测
    if(tel.size()!=_DEF_TEL_LEN){
        ui->LB_ReplyShow->setText("电话号码长度错误");
        return;
    }
    //非法字符检测
    for(QChar v:tel){
        if(!(v>='0'&&v<='9'))
        {
            ui->LB_ReplyShow->setText("电话号码有非法字符");
            return;
        }
    }
    //pswd
    //长度检测
    if(pswd.size()<6){
        ui->LB_ReplyShow->setText("密码太短小于六位");
        return;
    }
    if(pswd.size()>_DEF_PSWD_LEN){
        ui->LB_ReplyShow->setText("密码太长大于六位");
        return;
    }
    //非法字符检测
    for(QChar v:pswd){
        if('!'==v||'#'==v||'$'==v||'%'==v||'^'==v||'&'==v||'*'==v||'('==v||')'==v||'-'==v||' '==v)
        {
            ui->LB_ReplyShow->setText("密码有非法字符");
            return;
        }
    }
    if(QString::compare(pswd,ensure)){
        ui->LB_ReplyShow->setText("两次输入的密码不相同");
        return;
    }
    Msg_Register* reg=new Msg_Register;
    pswd=QString("%1_MD5KEY").arg(pswd);
    reg->m_type=_DEF_TCP_REGISTER;
    strcpy(reg->m_reg.m_name,name.toStdString().c_str());
    strcpy(reg->m_reg.m_pswd,m_pMed->MD5_encrypt(pswd.toStdString().c_str()).c_str());
    strcpy(reg->m_reg.m_tel,tel.toStdString().c_str());
    m_pMed->send((char*)reg,sizeof(Msg_Register),0);
}

void Dialog::showReply(char* buf){

    if(nullptr==buf) return;
    //ui->LB_ReplyShow->clear();
    ui->LB_ReplyShow->setText(QString(buf));
}

