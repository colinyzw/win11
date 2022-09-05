/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QLabel *lb_icon;
    QTabWidget *tabWidget;
    QWidget *page_login;
    QLabel *lb_tel;
    QLabel *lb_password;
    QLineEdit *le_tel;
    QLineEdit *le_password;
    QPushButton *pb_clear;
    QPushButton *pb_commit;
    QWidget *page_register;
    QLabel *lb_tel_register;
    QLabel *lb_password_register;
    QLineEdit *le_tel_register;
    QLineEdit *le_password_register;
    QPushButton *pb_clear_register;
    QPushButton *pb_commit_register;
    QLabel *lb_name_register;
    QLineEdit *le_name_register;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QStringLiteral("LoginDialog"));
        LoginDialog->resize(600, 450);
        LoginDialog->setMinimumSize(QSize(600, 450));
        LoginDialog->setMaximumSize(QSize(600, 450));
        lb_icon = new QLabel(LoginDialog);
        lb_icon->setObjectName(QStringLiteral("lb_icon"));
        lb_icon->setGeometry(QRect(0, 0, 250, 450));
        lb_icon->setPixmap(QPixmap(QString::fromUtf8(":/images/register.png")));
        lb_icon->setScaledContents(true);
        tabWidget = new QTabWidget(LoginDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(250, 0, 361, 451));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        tabWidget->setFont(font);
        page_login = new QWidget();
        page_login->setObjectName(QStringLiteral("page_login"));
        lb_tel = new QLabel(page_login);
        lb_tel->setObjectName(QStringLiteral("lb_tel"));
        lb_tel->setGeometry(QRect(60, 110, 54, 31));
        lb_password = new QLabel(page_login);
        lb_password->setObjectName(QStringLiteral("lb_password"));
        lb_password->setGeometry(QRect(60, 160, 54, 31));
        le_tel = new QLineEdit(page_login);
        le_tel->setObjectName(QStringLiteral("le_tel"));
        le_tel->setGeometry(QRect(130, 110, 141, 31));
        le_password = new QLineEdit(page_login);
        le_password->setObjectName(QStringLiteral("le_password"));
        le_password->setGeometry(QRect(130, 160, 141, 31));
        le_password->setEchoMode(QLineEdit::Password);
        pb_clear = new QPushButton(page_login);
        pb_clear->setObjectName(QStringLiteral("pb_clear"));
        pb_clear->setGeometry(QRect(60, 270, 75, 31));
        pb_commit = new QPushButton(page_login);
        pb_commit->setObjectName(QStringLiteral("pb_commit"));
        pb_commit->setGeometry(QRect(200, 270, 75, 31));
        tabWidget->addTab(page_login, QString());
        page_register = new QWidget();
        page_register->setObjectName(QStringLiteral("page_register"));
        lb_tel_register = new QLabel(page_register);
        lb_tel_register->setObjectName(QStringLiteral("lb_tel_register"));
        lb_tel_register->setGeometry(QRect(40, 80, 61, 31));
        lb_password_register = new QLabel(page_register);
        lb_password_register->setObjectName(QStringLiteral("lb_password_register"));
        lb_password_register->setGeometry(QRect(40, 180, 61, 31));
        le_tel_register = new QLineEdit(page_register);
        le_tel_register->setObjectName(QStringLiteral("le_tel_register"));
        le_tel_register->setGeometry(QRect(120, 80, 151, 31));
        le_password_register = new QLineEdit(page_register);
        le_password_register->setObjectName(QStringLiteral("le_password_register"));
        le_password_register->setGeometry(QRect(120, 180, 151, 31));
        QFont font1;
        font1.setPointSize(2);
        le_password_register->setFont(font1);
        le_password_register->setEchoMode(QLineEdit::Password);
        pb_clear_register = new QPushButton(page_register);
        pb_clear_register->setObjectName(QStringLiteral("pb_clear_register"));
        pb_clear_register->setGeometry(QRect(60, 270, 75, 31));
        pb_commit_register = new QPushButton(page_register);
        pb_commit_register->setObjectName(QStringLiteral("pb_commit_register"));
        pb_commit_register->setGeometry(QRect(200, 270, 75, 31));
        lb_name_register = new QLabel(page_register);
        lb_name_register->setObjectName(QStringLiteral("lb_name_register"));
        lb_name_register->setGeometry(QRect(40, 130, 61, 31));
        le_name_register = new QLineEdit(page_register);
        le_name_register->setObjectName(QStringLiteral("le_name_register"));
        le_name_register->setGeometry(QRect(120, 130, 151, 31));
        tabWidget->addTab(page_register, QString());

        retranslateUi(LoginDialog);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QApplication::translate("LoginDialog", "Dialog", 0));
        lb_icon->setText(QString());
        lb_tel->setText(QApplication::translate("LoginDialog", "\346\211\213\346\234\272\345\217\267", 0));
        lb_password->setText(QApplication::translate("LoginDialog", "\345\257\206\347\240\201", 0));
#ifndef QT_NO_TOOLTIP
        le_tel->setToolTip(QApplication::translate("LoginDialog", "<html><head/><body><p>\346\211\213\346\234\272\345\217\267\344\270\215\350\203\275\350\266\205\350\277\20711\344\275\215</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        le_tel->setText(QApplication::translate("LoginDialog", "12345123451", 0));
#ifndef QT_NO_TOOLTIP
        le_password->setToolTip(QApplication::translate("LoginDialog", "<html><head/><body><p>\345\257\206\347\240\201\344\270\215\350\203\275\350\266\205\350\277\20710\344\275\215</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        le_password->setText(QApplication::translate("LoginDialog", "123", 0));
        pb_clear->setText(QApplication::translate("LoginDialog", "\346\270\205\347\251\272", 0));
        pb_commit->setText(QApplication::translate("LoginDialog", "\346\217\220\344\272\244", 0));
        tabWidget->setTabText(tabWidget->indexOf(page_login), QApplication::translate("LoginDialog", " \347\231\273\345\275\225 ", 0));
        lb_tel_register->setText(QApplication::translate("LoginDialog", "\346\211\213\346\234\272\345\217\267", 0));
        lb_password_register->setText(QApplication::translate("LoginDialog", "\345\257\206\347\240\201", 0));
#ifndef QT_NO_TOOLTIP
        le_tel_register->setToolTip(QApplication::translate("LoginDialog", "<html><head/><body><p>\346\211\213\346\234\272\345\217\267\344\270\215\350\203\275\350\266\205\350\277\20711\344\275\215</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        le_password_register->setToolTip(QApplication::translate("LoginDialog", "<html><head/><body><p>\345\257\206\347\240\201\344\270\215\350\203\275\350\266\205\350\277\20710\344\275\215</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        pb_clear_register->setText(QApplication::translate("LoginDialog", "\346\270\205\347\251\272", 0));
        pb_commit_register->setText(QApplication::translate("LoginDialog", "\346\217\220\344\272\244", 0));
        lb_name_register->setText(QApplication::translate("LoginDialog", "\346\230\265\347\247\260", 0));
#ifndef QT_NO_TOOLTIP
        le_name_register->setToolTip(QApplication::translate("LoginDialog", "<html><head/><body><p>\346\230\265\347\247\260\344\270\215\350\203\275\350\266\205\350\277\20720\344\275\215</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        tabWidget->setTabText(tabWidget->indexOf(page_register), QApplication::translate("LoginDialog", " \346\263\250\345\206\214 ", 0));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
