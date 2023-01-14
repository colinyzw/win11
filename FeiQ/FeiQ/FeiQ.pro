#-------------------------------------------------
#
# Project created by QtCreator 2021-11-29T16:12:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FeiQ
TEMPLATE = app
#
INCLUDEPATH += ./net
INCLUDEPATH += ./mediator
#依赖�
LIBS += -lws2_32
QT += network

SOURCES += main.cpp\
        feiq.cpp \
    net/UdpNet.cpp \
    mediator/NetMediator.cpp \
    mediator/UdpMediator.cpp \
    ckernel.cpp \
    chatdialog.cpp

HEADERS  += feiq.h \
    net/INet.h \
    net/Package.h \
    net/UdpNet.h \
    mediator/INetMediator.h \
    mediator/UdpMediator.h \
    ckernel.h \
    chatdialog.h

FORMS    += feiq.ui \
    chatdialog.ui
