#include "mainwindow.h"
#include <QApplication>

Animation* animation = new Animation;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //TODO: break out a constant.
    animation->AddFrame(25);

    MainWindow w;
    w.show();
    w.setAnimation(animation);

    return app.exec();
}
