#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mainmenu.h"
#include "frame.h"

class MainWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setAnimation(Animation* animation);

private:
    MainMenu _mainMenu;
};

#endif // MAINWINDOW_H
