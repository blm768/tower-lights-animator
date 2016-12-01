#include "mainwindow.h"
#include "frameeditor.h"
#include "timeline.h"
#include "colorpicker.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _mainMenu(this)
{
    QWidget* mainBox = new QWidget;
    QWidget* hozBox = new QWidget;
    QBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* hozLayout = new QHBoxLayout;

    hozBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    mainBox->setLayout(mainLayout);
    hozBox->setLayout(hozLayout);

    setCentralWidget(mainBox);

    FrameEditor* frameEditor = new FrameEditor(this);
    hozLayout->addWidget(frameEditor);

    ColorPicker* colorPick = new ColorPicker(this);
    hozLayout->addWidget(colorPick);





    Timeline* timeline = new Timeline(this);
    mainLayout->addWidget(hozBox);
    mainLayout->addWidget(timeline);
    connect(this, &MainWindow::animationSet, timeline, &Timeline::setAnimation);
    connect(this, &MainWindow::animationSet, frameEditor, &FrameEditor::setAnimation);
    connect(frameEditor, &FrameEditor::frameChanged, timeline, &Timeline::onFrameChanged);

    connect(colorPick, SIGNAL(sendColor(QColor)), frameEditor, SLOT(setPenColor(QColor)));

    connect(timeline, &Timeline::selectionChanged, frameEditor, &FrameEditor::setSelection);

}

MainWindow::~MainWindow()
{

}

// TODO: free the old animation after sending animationSet.
void MainWindow::setAnimation(Animation *animation) {
    _animation = animation;
    animationSet(animation);
}
