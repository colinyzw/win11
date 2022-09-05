#-------------------------------------------------
#
# Project created by QtCreator 2022-01-17T23:05:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyChat
TEMPLATE = app

#依赖库和添加依赖路径
LIBS += -lws2_32
INCLUDEPATH += ./net
INCLUDEPATH += ./mediator

SOURCES += main.cpp\
        mychatdialog.cpp \
    net/TcpClientNet.cpp \
    mediator/TcpClientMediator.cpp \
    ckernel.cpp \
    mediator/INetMediator.cpp \
    logindialog.cpp \
    useritem.cpp \
    chatdialog.cpp

HEADERS  += mychatdialog.h \
    net/INet.h \
    net/Package.h \
    net/TcpClientNet.h \
    mediator/INetMediator.h \
    mediator/TcpClientMediator.h \
    ckernel.h \
    logindialog.h \
    useritem.h \
    chatdialog.h

FORMS    += mychatdialog.ui \
    logindialog.ui \
    useritem.ui \
    chatdialog.ui

RESOURCES += \
    resource.qrc
