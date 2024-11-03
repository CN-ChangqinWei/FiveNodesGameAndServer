#include "dialog.h"
#include <QApplication>
#include"Kernel.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CKernel ker;
    return a.exec();
}
