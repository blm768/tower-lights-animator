#include "mainwindow.h"
#include "frameeditor.h"
#include "timeline.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _mainMenu(this)
{
    //Timeline* timeline = new Timeline(this);
    //setCentralWidget(timeline);
    FrameEditor* frameeditor = new FrameEditor(this);
    setCentralWidget(frameeditor);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setAnimation(Animation *animation) {

}
