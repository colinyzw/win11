#ifndef FEIQ_H
#define FEIQ_H
#include<QString>
#include <QDialog>
#include<QCloseEvent>
#include<Package.h>

namespace Ui {
class FeiQ;
}

class FeiQ : public QDialog
{
    Q_OBJECT

public:
    explicit FeiQ(QWidget *parent = 0);
    ~FeiQ();

public slots:
    void slot_addFriend(QString ip,QString host);
    void slot_deleteFriend(QString ip);

private slots:
    void on_tw_friend_cellDoubleClicked(int row, int column);
    void closeEvent(QCloseEvent* event);

signals:
    void SIG_userClicked(QString ip);
    void SIG_close();

private:
    Ui::FeiQ *ui;
};

#endif // FEIQ_H
