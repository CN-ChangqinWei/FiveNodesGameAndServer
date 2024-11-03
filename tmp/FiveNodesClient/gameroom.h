#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <QDialog>
#include"protocl.h"
#include"fivenodeswidget.h"
#define _DEF_TIMERELOAD (30)
class CTCPMediator;
namespace Ui {
class GameRoom;
}

class GameRoom : public QDialog
{
    Q_OBJECT

public:
    explicit GameRoom(CTCPMediator* _med);
    ~GameRoom();
    void closeEvent(QCloseEvent *event);
    void refresh(GameRoomInfo info);
    int get_roomid();
    void ch_gameturn();
public slots:
    void slot_openroom(int roomid);
    void slot_refresh(GameRoomInfo info);
    void slot_gamewin(Pieces color);
    void slot_startgame();
    void slot_piecedown(QPoint point,Pieces color);
    void slot_chturn();
    void slot_count();
private slots:
    void on_PB_AImode_clicked();

    void on_pushButton_clicked();

    void on_PB_Start_clicked();

private:
    Ui::GameRoom *ui;
    CTCPMediator* m_pMed;
    FiveNodesWidget* m_pGame;
    QTimer m_timer;
    int m_counter;
    int m_RoomID;
    int m_exist[2];
    bool m_stat;
};

#endif // GAMEROOM_H
