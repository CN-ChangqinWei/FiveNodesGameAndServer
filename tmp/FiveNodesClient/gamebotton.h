#ifndef GAMEBOTTON_H
#define GAMEBOTTON_H
#include "hallconfig.h"
#include <QWidget>
#include <QColor>
namespace Ui {
class GameBotton;
}

class GameBotton : public QWidget
{
    Q_OBJECT

public:
    explicit GameBotton(int _id,const char* _path);
    ~GameBotton();
    void SetBottonColor(QPalette color);
signals:
    void sig_gamechange(int id);
private slots:
    void on_PB_Game_clicked();

private:
    Ui::GameBotton *ui;
    int m_GameID;
};

#endif // GAMEBOTTON_H
