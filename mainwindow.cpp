#include "mainwindow.h"
#include "frameeditor.h"
#include "timeline.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _mainMenu(this)
{
    QWidget* mainBox = new QWidget;
    QBoxLayout* mainLayout = new QVBoxLayout;
    mainBox->setLayout(mainLayout);
    setCentralWidget(mainBox);

    FrameEditor* frameEditor = new FrameEditor(this);
    mainLayout->addWidget(frameEditor);

    Timeline* timeline = new Timeline(this);
    mainLayout->addWidget(timeline);
    connect(this, &MainWindow::animationSet, timeline, &Timeline::setAnimation);
    connect(frameEditor, &FrameEditor::frameChanged, timeline, &Timeline::onFrameChanged);
}

MainWindow::~MainWindow()
{

}

// TODO: free the old animation after sending animationSet.
void MainWindow::setAnimation(Animation *animation) {
    _animation = animation;
    animationSet(animation);
}
