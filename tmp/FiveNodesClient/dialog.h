#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "./Mediator/TCPMediator.h"
#include"protocl.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setMediator(CTCPMediator* _med);
    void showReply(char* buf);
public slots:
    void slot_show(char* buf);
private slots:
   // void on_PB_Send_clicked();

    void on_PB_GetTime_clicked();

    void on_PB_LoginClear_clicked();

    void on_PB_LoginSubit_clicked();

    void on_PB_RegisterClear_clicked();

    void on_PB_RegisterSubit_clicked();

private:
    Ui::Dialog *ui;
    CTCPMediator* m_pMed;
};

#endif // DIALOG_H
