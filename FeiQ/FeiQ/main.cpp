#include "feiq.h"
#include <QApplication>
#include<ckernel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //FeiQ w;
    //w.show();
    CKernel kernel;

    return a.exec();
}
