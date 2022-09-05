#include "mychatdialog.h"
#include <QApplication>
#include"ckernel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MyChatDialog w;
//    w.show();
    //创建kernel类对象
    CKernel kernel;

    return a.exec();
}
