#include "mainwindow.h"

#include "timeline.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _mainMenu(this)
{
    Timeline* timeline = new Timeline(this);
    setCentralWidget(timeline);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setAnimation(Animation *animation) {

}
