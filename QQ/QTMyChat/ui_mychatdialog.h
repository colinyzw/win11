/********************************************************************************
** Form generated from reading UI file 'mychatdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYCHATDIALOG_H
#define UI_MYCHATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyChatDialog
{
public:
    QPushButton *pb_icon;
    QLabel *lb_name;
    QLineEdit *le_feeling;
    QTabWidget *tabWidget;
    QWidget *page_friend;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QWidget *wdg_list;
    QSpacerItem *verticalSpacer;
    QWidget *page_group;
    QWidget *page_space;
    QPushButton *pb_menu;
    QPushButton *pb_tool1;
    QPushButton *pb_tool2;
    QPushButton *pb_tool3;

    void setupUi(QDialog *MyChatDialog)
    {
        if (MyChatDialog->objectName().isEmpty())
            MyChatDialog->setObjectName(QStringLiteral("MyChatDialog"));
        MyChatDialog->resize(300, 600);
        MyChatDialog->setMinimumSize(QSize(300, 600));
        MyChatDialog->setMaximumSize(QSize(300, 600));
        MyChatDialog->setSizeIncrement(QSize(0, 0));
        pb_icon = new QPushButton(MyChatDialog);
        pb_icon->setObjectName(QStringLiteral("pb_icon"));
        pb_icon->setGeometry(QRect(10, 10, 60, 60));
        QIcon icon;
        icon.addFile(QStringLiteral(":/tx/13.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_icon->setIcon(icon);
        pb_icon->setIconSize(QSize(60, 60));
        pb_icon->setFlat(true);
        lb_name = new QLabel(MyChatDialog);
        lb_name->setObjectName(QStringLiteral("lb_name"));
        lb_name->setGeometry(QRect(90, 10, 211, 30));
        le_feeling = new QLineEdit(MyChatDialog);
        le_feeling->setObjectName(QStringLiteral("le_feeling"));
        le_feeling->setGeometry(QRect(90, 40, 191, 28));
        tabWidget = new QTabWidget(MyChatDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 70, 281, 491));
        tabWidget->setMinimumSize(QSize(281, 491));
        tabWidget->setMaximumSize(QSize(281, 491));
        page_friend = new QWidget();
        page_friend->setObjectName(QStringLiteral("page_friend"));
        verticalLayout = new QVBoxLayout(page_friend);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        scrollArea = new QScrollArea(page_friend);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 255, 446));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        wdg_list = new QWidget(scrollAreaWidgetContents);
        wdg_list->setObjectName(QStringLiteral("wdg_list"));

        verticalLayout_2->addWidget(wdg_list);

        verticalSpacer = new QSpacerItem(20, 409, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        tabWidget->addTab(page_friend, QString());
        page_group = new QWidget();
        page_group->setObjectName(QStringLiteral("page_group"));
        tabWidget->addTab(page_group, QString());
        page_space = new QWidget();
        page_space->setObjectName(QStringLiteral("page_space"));
        tabWidget->addTab(page_space, QString());
        pb_menu = new QPushButton(MyChatDialog);
        pb_menu->setObjectName(QStringLiteral("pb_menu"));
        pb_menu->setGeometry(QRect(10, 560, 30, 30));
        pb_tool1 = new QPushButton(MyChatDialog);
        pb_tool1->setObjectName(QStringLiteral("pb_tool1"));
        pb_tool1->setGeometry(QRect(180, 560, 30, 30));
        pb_tool2 = new QPushButton(MyChatDialog);
        pb_tool2->setObjectName(QStringLiteral("pb_tool2"));
        pb_tool2->setGeometry(QRect(220, 560, 30, 30));
        pb_tool3 = new QPushButton(MyChatDialog);
        pb_tool3->setObjectName(QStringLiteral("pb_tool3"));
        pb_tool3->setGeometry(QRect(260, 560, 30, 30));

        retranslateUi(MyChatDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MyChatDialog);
    } // setupUi

    void retranslateUi(QDialog *MyChatDialog)
    {
        MyChatDialog->setWindowTitle(QApplication::translate("MyChatDialog", "MyChatDialog", 0));
        pb_icon->setText(QString());
        lb_name->setText(QApplication::translate("MyChatDialog", "Brooklyn.", 0));
        tabWidget->setTabText(tabWidget->indexOf(page_friend), QApplication::translate("MyChatDialog", "  \345\245\275\345\217\213  ", 0));
        tabWidget->setTabText(tabWidget->indexOf(page_group), QApplication::translate("MyChatDialog", "  \347\276\244\347\273\204 ", 0));
        tabWidget->setTabText(tabWidget->indexOf(page_space), QApplication::translate("MyChatDialog", " \347\251\272\351\227\264  ", 0));
        pb_menu->setText(QString());
        pb_tool1->setText(QString());
        pb_tool2->setText(QString());
        pb_tool3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MyChatDialog: public Ui_MyChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYCHATDIALOG_H
