#include "mainwindow.h"
#include <QApplication>
#include "tanIO.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    Animation  * animation = new Animation;
    //LoadTan("/home/kwben_000/tower-lights-animator/untitled.tan", animation);
}
