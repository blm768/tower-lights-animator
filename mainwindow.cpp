#include "mainwindow.h"

#include "timeline.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Timeline* timeline = new Timeline(this);
    setCentralWidget(timeline);
}

MainWindow::~MainWindow()
{

}
