#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QDialog>
#include"protocl.h"
class CTCPMediator;
namespace Ui {
class FriendList;
}

class FriendList : public QDialog
{
    Q_OBJECT

public:
    explicit FriendList(CTCPMediator* _med);
    ~FriendList();
    void closeEvent(QCloseEvent* _event);
    void init(CTCPMediator* _med);
public slots:
    void slot_userinfo(UserInfo info);
    void slot_friendinfo(UserInfo info);
private:
    Ui::FriendList *ui;
    CTCPMediator* m_pMed;
};

#endif // FRIENDLIST_H
