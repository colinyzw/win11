/********************************************************************************
** Form generated from reading UI file 'useritem.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERITEM_H
#define UI_USERITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserItem
{
public:
    QPushButton *pb_icon;
    QLabel *lb_name;
    QLabel *lb_feeling;

    void setupUi(QWidget *UserItem)
    {
        if (UserItem->objectName().isEmpty())
            UserItem->setObjectName(QStringLiteral("UserItem"));
        UserItem->resize(270, 60);
        UserItem->setMinimumSize(QSize(260, 60));
        pb_icon = new QPushButton(UserItem);
        pb_icon->setObjectName(QStringLiteral("pb_icon"));
        pb_icon->setGeometry(QRect(10, 10, 40, 40));
        pb_icon->setIconSize(QSize(40, 40));
        pb_icon->setFlat(true);
        lb_name = new QLabel(UserItem);
        lb_name->setObjectName(QStringLiteral("lb_name"));
        lb_name->setGeometry(QRect(60, 10, 201, 22));
        lb_feeling = new QLabel(UserItem);
        lb_feeling->setObjectName(QStringLiteral("lb_feeling"));
        lb_feeling->setGeometry(QRect(60, 30, 201, 22));

        retranslateUi(UserItem);

        QMetaObject::connectSlotsByName(UserItem);
    } // setupUi

    void retranslateUi(QWidget *UserItem)
    {
        UserItem->setWindowTitle(QApplication::translate("UserItem", "Form", 0));
        pb_icon->setText(QString());
        lb_name->setText(QApplication::translate("UserItem", "111", 0));
        lb_feeling->setText(QApplication::translate("UserItem", "222", 0));
    } // retranslateUi

};

namespace Ui {
    class UserItem: public Ui_UserItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERITEM_H
