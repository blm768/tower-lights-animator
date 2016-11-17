#include "mainwindow.h"
#include <QApplication>
#include "tanIO.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Animation* animation = new Animation;

    MainWindow w;
    w.show();
    w.setAnimation(animation);

    return app.exec();

    //LoadTan("/home/kwben_000/tower-lights-animator/untitled.tan", animation);
}
