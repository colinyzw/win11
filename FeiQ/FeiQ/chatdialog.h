#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>
#include<QString>
namespace Ui {
class chatDialog;
}

class chatDialog : public QWidget
{
    Q_OBJECT

public:
    explicit chatDialog(QWidget *parent = 0);
    ~chatDialog();
    void setInfo(const QString &ip);

private slots:
    void on_pb_send_clicked();

public slots:
    void slot_SetChatMsg(QString content);

signals:
    void SIG_SendChatMsg(QString ip,QString content);

private:
    Ui::chatDialog *ui;
    QString m_ip;
};

#endif // CHATDIALOG_H
