#include <QCoreApplication>
#include <QtGui/QColor>
#include <QTime>
#include <QList>
#include <iostream>
#include "towerframe.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TowerFrame Tower;
    Tower.AddFrame(QTime(0,0,0,200));
    Tower.AddFrame(QTime(0,0,0,300));
    Tower.AddFrame(QTime(0,0,0,200));
    Tower.AddFrame(QTime(0,0,0,200));
    Tower.MoveFrame(3,0);
    Tower.ColorCell(4,10,10,QColor(255,255,255,0));

    Tower.PrintTower();
    std::cout << Tower.GetDuration() << std::endl;
    return a.exec();
}
