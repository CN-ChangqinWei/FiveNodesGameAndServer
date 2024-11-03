#ifndef ROOM_H
#define ROOM_H

#include <QWidget>
class CTCPMediator;
namespace Ui {
class room;
}

class room : public QWidget
{
    Q_OBJECT
signals:
    void sig_getplayerinfo(int roomid,int roomcode);
    void sig_joingame(int roomid,int roomcode);
public:
    explicit room(int id,int zoneid,CTCPMediator* _med,QWidget *parent = 0);
    ~room();
    void init(int id,int zoneid,CTCPMediator* _med);
    void delplayer(int id);
    void refresh(int icid0,int icid1);
    bool get_stat();
private slots:
    void on_PB_Play1HeadIcon_clicked();

    void on_PB_Play2HeadIcon_clicked();

private:
    Ui::room *ui;
    CTCPMediator* m_pMed;
    int m_roomid;
    int m_zoneid;
    //bool m_stat;
    bool m_exist[2];
    bool m_isrun;

};

#endif // ROOM_H
