#ifndef FIVENODESWIDGET_H
#define FIVENODESWIDGET_H

#include <QWidget>
#include<QPainter>
#include<vector>
#include<QTimer>
using namespace  std ;
#define _DEF_NOFLINES   (15)
#define _DEF_SPACE      (width()/(_DEF_NOFLINES+2))
#define _DEF_LINESPACE   _DEF_SPACE       //((width()-2*_DEF_SPACE)/_DEF_NOFLINES)
#define _DEF_COLOR_LINE (QColor(0,0,0))
#define _DEF_BLACK      (QColor(0,0,0))
#define _DEF_WHITE      (QColor(255,255,255))
#define _DEF_PIECES_SIZERATE    (0.3)
#define _DEF_FPS(fps)                (1000/fps)
#define _DEF_REFLECT(space,x)   (space+x*space)
#define _DEF_UNREFLECT(space,x) ((x-space/2)/space)

enum Pieces{
  none=0,
  black,
   white

};

namespace Ui {
class FiveNodesWidget;
}

class FiveNodesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FiveNodesWidget(QWidget *parent = 0);
    ~FiveNodesWidget();
    void init();
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    Pieces get_color();
    Pieces get_turn();
    int isRun();
    void restart();
    void ch_turn();
private:
    void setdown_piece(QPoint point,Pieces color);
    void judge_winner(QPoint point,Pieces color);
    int dfs_getnum(QPoint point,int dir_x,int dir_y,Pieces color);
    void act_AI();

signals:
    void sig_playerwin(Pieces color);
    void sig_pciecedown(QPoint point,Pieces color);
public slots:
    void slot_turnchange(Pieces color);
    void slot_piecesdown(Pieces color);
    void slot_set_color(Pieces color);
    void slot_set_AIcolor(Pieces color);
    void slot_set_turn(Pieces color);
    void slot_start();
    void slot_close();
    void slot_close_AI();
    void slot_open_AI();
    void slot_CHAIStat();
    void slot_setdownpieces(QPoint point,Pieces color);
private:
    static char m_dirx[4];
    static char m_diry[4];
    static int m_score_en[4];
    static int m_score_mine[4];
    Ui::FiveNodesWidget *ui;
    bool m_SetPieces;
    Pieces m_turn;
    Pieces m_color;
    Pieces m_AIcolor;
    bool m_isRun;
    bool m_isAIuse;
    QPoint m_piece;
    vector<vector<Pieces>> m_chessboard;
    vector<vector<int>> m_countboard;
    int m_minx;
    int m_miny;
    int m_maxx;
    int m_maxy;
    int m_space;
    int m_count;
    int m_maxnodes;
    int m_nofnodes;
    QTimer m_timer;
};

#endif // FIVENODESWIDGET_H
