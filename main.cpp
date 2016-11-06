#include "mainwindow.h"
#include <QApplication>
#include "tanIO.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //return a.exec();

    TowerFrame  * animation;
    LoadTan("/home/paden/Desktop/cs383/testTan1.tan", animation);
}
