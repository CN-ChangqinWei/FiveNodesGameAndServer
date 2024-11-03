#include "fivenodeswidget.h"
#include "ui_fivenodeswidget.h"
#include"QPainter"
#include<QMouseEvent>
#include<QDebug>
#include"gameroom.h"
#include <synchapi.h>
#include<QFontMetrics>
FiveNodesWidget::FiveNodesWidget(QWidget *parent) :
    QWidget(parent),m_timer(this),
    ui(new Ui::FiveNodesWidget)
{
    ui->setupUi(this);
    init();

}
void FiveNodesWidget::init(){
    setAttribute(Qt::WA_StyledBackground);
    m_isRun=false;
    m_count=0;
    m_chessboard=vector<vector<Pieces>>(_DEF_NOFLINES+1,vector<Pieces>(_DEF_NOFLINES+1,none));
    m_countboard=vector<vector<int>>(_DEF_NOFLINES+1,vector<int>(_DEF_NOFLINES+1,0));
    m_space=_DEF_SPACE;
    m_miny=m_minx=_DEF_SPACE/2;
    m_maxx=m_maxy=_DEF_SPACE*(_DEF_NOFLINES+1)+_DEF_SPACE/2;
    m_color=black;
    m_turn=black;
    m_SetPieces=false;
    m_isAIuse=false;
    m_timer.setInterval(_DEF_FPS(50));
    m_maxnodes=(_DEF_NOFLINES+1)*(_DEF_NOFLINES+1);
    m_nofnodes=0;
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(repaint()));
    //connect(this,SIGNAL(sig_pciecedown(Pieces)),this,SLOT(slot_piecesdown(Pieces)));

    m_timer.start();
}
void FiveNodesWidget::paintEvent(QPaintEvent* event){
    //if(m_isRun==false){event->accept();return;}
    QPainter paint(this);
    paint.setBrush(QBrush(_DEF_COLOR_LINE));
    for(int i=0;i<=_DEF_NOFLINES;i++){
        QPoint p1(m_space+i*m_space,m_space);
        QPoint p2(m_space+i*m_space,m_space*(_DEF_NOFLINES+1));
        paint.drawLine(p1,p2);
        QPoint p3(m_space,m_space+i*m_space);
        QPoint p4(m_space*(_DEF_NOFLINES+1),m_space+i*m_space);
        paint.drawLine(p3,p4);
    }
    if(m_SetPieces&&!(m_isAIuse&&m_AIcolor==m_turn))
    {
        if(m_color==black)
        paint.setBrush(_DEF_BLACK);
        else paint.setBrush(_DEF_WHITE);
        paint.drawEllipse(m_piece,m_space*_DEF_PIECES_SIZERATE,m_space*_DEF_PIECES_SIZERATE);
    }

    for(int i=0;i<m_chessboard.size();i++){
        for(int j=0;j<m_chessboard[0].size();j++){
            if(m_chessboard[i][j]!=none){
                paint.setPen(QColor(Qt::black));
                if(m_chessboard[i][j]==black)
                paint.setBrush(_DEF_BLACK);
                else paint.setBrush(_DEF_WHITE);
                QPoint point(_DEF_REFLECT(m_space,j),_DEF_REFLECT(m_space,i));
                paint.drawEllipse(point,m_space*_DEF_PIECES_SIZERATE,m_space*_DEF_PIECES_SIZERATE);
                if(m_chessboard[i][j]==black)
                paint.setBrush(_DEF_WHITE);
                else paint.setBrush(_DEF_BLACK);
                char text[10]={0};
                itoa(m_countboard[i][j],text,10);
                QString str(text);
                paint.setPen(QColor(Qt::red));
                QFontMetrics fontmertrics(paint.font());
                point=QPoint(point.x()-fontmertrics.width(str)/2,point.y());
                paint.drawText(point,str);
            }

        }
    }

    //qDebug()<<"paint"<<endl;
    event->accept();
}


void FiveNodesWidget::mousePressEvent(QMouseEvent *event){
    if(m_isRun==false||(m_isAIuse&&m_AIcolor==m_turn)){event->accept();return;}
    //qDebug()<<"mouse pressed"<<endl;
    if(m_turn==m_color){
        m_SetPieces=true;
        m_piece=event->pos();
    }
    event->accept();

}
void FiveNodesWidget::mouseMoveEvent(QMouseEvent *event){
    if(m_isRun==false||(m_isAIuse&&m_AIcolor==m_turn)){m_SetPieces=false;event->accept();return;}
   // qDebug()<<"mouse move"<<endl;
    if(m_SetPieces)
    m_piece=event->pos();
    event->accept();

}
void FiveNodesWidget::mouseReleaseEvent(QMouseEvent *event){
    if(m_isRun==false||(m_isAIuse&&m_AIcolor==m_turn)){m_SetPieces=false;event->accept();return;}
    //qDebug()<<"mouse release"<<endl;
    if(m_SetPieces){
        m_piece=event->pos();
        setdown_piece(m_piece,m_color);

    }
    m_SetPieces=false;
    event->accept();
}
void FiveNodesWidget::slot_set_color(Pieces color){
    if(color!=black||color!=white) m_color=color;

}
void FiveNodesWidget::slot_set_turn(Pieces color){
    m_turn=color;

}
void FiveNodesWidget::slot_turnchange(Pieces color){


}
void FiveNodesWidget::setdown_piece(QPoint point,Pieces color){
    if(!m_isRun||m_turn!=color||m_nofnodes>=m_maxnodes) return;
    int x=point.x();
    int y=point.y();
    if(x<m_minx||x>m_maxx||y<m_miny||y>m_maxy){
        return;
    }
    int i=_DEF_UNREFLECT(m_space,y);
    int j=_DEF_UNREFLECT(m_space,x);
    if(i<0||i>_DEF_NOFLINES||j<0||j>_DEF_NOFLINES) return;
    if(none!=m_chessboard[i][j]) return;
    m_chessboard[i][j]=color;
    m_countboard[i][j]=++m_count;
    m_nofnodes++;

    if(color==m_color){
        emit sig_pciecedown(point,color);
        qDebug()<<"emit signals";
    }
    judge_winner(QPoint(j,i),color);
    if(m_turn==black) m_turn=white;
    else m_turn=black;
    act_AI();

}

void FiveNodesWidget::slot_setdownpieces(QPoint point,Pieces color){
    setdown_piece(point,color);

}

void FiveNodesWidget::slot_start(){
     m_isRun=true;
     m_turn=black;
     m_nofnodes=0;
    m_timer.start();

}

void FiveNodesWidget::slot_close(){
    m_isRun=false;
    //restart();
    //m_timer.stop();


}
void FiveNodesWidget::slot_piecesdown(Pieces color){
    //----------测试用-----------
//    if(color==black) m_color=m_turn=white;
//    else m_color=m_turn=black;
    //-----------
}
void FiveNodesWidget::judge_winner(QPoint point,Pieces color){
    int x=point.x();
    int y=point.y();
    if(x<0||x>_DEF_NOFLINES||y<0||y>_DEF_NOFLINES) return;
    for(int i=0;i<4;i++){
        int nums=1+dfs_getnum(QPoint(x+m_dirx[i],y+m_diry[i]),m_dirx[i],m_diry[i],color)+dfs_getnum(QPoint(x-m_dirx[i],y-m_diry[i]),-m_dirx[i],-m_diry[i],color);
        //qDebug()<<nums<<endl;
        if(nums>=5){
           // ui->LB_Winner->setText(QString(""));
            if(black==color)    ui->LB_Winner->setText(QString("黑方胜利 %1").arg(nums));
            else    ui->LB_Winner->setText(QString("白方胜利 %1").arg(nums));
            qDebug()<<color<<"win type:"<<i<<endl;
            m_isRun=false;
            emit sig_playerwin(color);
            return;
        }
    }
    if(m_nofnodes>=m_maxnodes){
        emit sig_playerwin(none);
        m_isRun=false;
        return;
    }
}
int FiveNodesWidget::dfs_getnum(QPoint point,int dir_x,int dir_y,Pieces color){
    int x=point.x();
    int y=point.y();
    if(x<0||x>_DEF_NOFLINES||y<0||y>_DEF_NOFLINES||m_chessboard[y][x]==none||m_chessboard[y][x]!=color) return 0;
    return dfs_getnum(QPoint(x+dir_x,y+dir_y),dir_x,dir_y,color)+1;
}
FiveNodesWidget::~FiveNodesWidget()
{
    close();
    delete ui;
}

void FiveNodesWidget::slot_set_AIcolor(Pieces color){
    m_AIcolor=color;
}
void FiveNodesWidget::slot_close_AI(){
    m_isAIuse=false;
    ui->LB_AIStat->setText(QString(""));
}
void FiveNodesWidget::slot_open_AI(){
    m_isAIuse=true;
    m_AIcolor=m_color;
    ui->LB_AIStat->setText(QString("已启动AI托管"));
}
void FiveNodesWidget::slot_CHAIStat(){
    m_AIcolor=m_color;
    //if(m_isAIuse)
    m_isAIuse=!m_isAIuse;
    if(m_isAIuse) ui->LB_AIStat->setText(QString("已启动AI托管"));
    else ui->LB_AIStat->setText(QString(""));
    act_AI();
}
void FiveNodesWidget::act_AI(){
    if(!m_isRun||!m_isAIuse||m_turn!=m_AIcolor) return;
    qDebug()<<"AI act";
    int maxx=_DEF_UNREFLECT(_DEF_SPACE,width()/2);
    int maxy=_DEF_UNREFLECT(_DEF_SPACE,height()/2);
    Pieces color_Mine=m_AIcolor;
    Pieces color_En;
    int maxscore=0;
    if(black==m_AIcolor) color_En=white;
    else color_En=black;
    for(int i=0;i<m_chessboard.size();i++){
        for(int j=0;j<m_chessboard[0].size();j++){
            if(m_chessboard[i][j]==none){

                for(int k=0;k<4;k++){
                    int nums_en=dfs_getnum(QPoint(j+m_dirx[k],i+m_diry[k]),m_dirx[k],m_diry[k],color_En)+dfs_getnum(QPoint(j-m_dirx[k],i-m_diry[k]),-m_dirx[k],-m_diry[k],color_En);
                    int nums_mine=dfs_getnum(QPoint(j+m_dirx[k],i+m_diry[k]),m_dirx[k],m_diry[k],color_Mine)+dfs_getnum(QPoint(j-m_dirx[k],i-m_diry[k]),-m_dirx[k],-m_diry[k],color_Mine);
                    int score_en=0,score_mine=0;
                    if(nums_en>0){
                        if(nums_en>5) nums_en=5;
                        score_en=m_score_en[nums_en-1];

                    }
                    if(nums_mine>0){
                        if(nums_mine>5) nums_mine=5;
                        score_mine=m_score_en[nums_mine-1];

                    }
                    int maxnum=max(nums_en,nums_mine);
                    if(maxnum>maxscore){
                        maxx=j;
                        maxy=i;
                        maxscore=maxnum;
                    }
                }

            }

        }

    }
    qDebug()<<maxx<<" "<<maxy<<" "<<maxscore;
    setdown_piece(QPoint(_DEF_REFLECT(_DEF_SPACE,maxx),_DEF_REFLECT(_DEF_SPACE,maxy)),m_color);
    Sleep(100);
}
Pieces FiveNodesWidget::get_color(){
    return m_color;
}
Pieces FiveNodesWidget::get_turn(){
    return m_turn;
}

void FiveNodesWidget::restart(){
    m_isRun=false;
    m_isAIuse=false;
    //m_chessboard.resize(_DEF_NOFLINES+1,vector<Pieces>(_DEF_NOFLINES+1,none));
    for(int i=0;i<m_chessboard.size();i++){
        for(int j=0;j<m_chessboard[0].size();j++){
            m_chessboard[i][j]=none;
            m_countboard[i][j]=0;
        }
    }
    m_turn=black;
}
int FiveNodesWidget::isRun(){
    return m_isRun;

}
void FiveNodesWidget::ch_turn(){
    if(black==m_turn) m_turn=white;
    else m_turn=black;

}
char FiveNodesWidget::m_dirx[4]={1,0,1,-1};
char FiveNodesWidget::m_diry[4]={0,1,1,1};
int FiveNodesWidget::m_score_en[4]={200,300,400,800};
int FiveNodesWidget::m_score_mine[4]={100,200,300,1000};

