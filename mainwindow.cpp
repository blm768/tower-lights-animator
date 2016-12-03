#include "mainwindow.h"
#include "frameeditor.h"
#include "timeline.h"
#include "colorpicker.h"
#include <QHBoxLayout>
#include "toolbar.h"

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

    Toolbar* toolbar = new Toolbar(this);
    hozLayout->addWidget(toolbar);

    Timeline* timeline = new Timeline(this);
    mainLayout->addWidget(hozBox);
    mainLayout->addWidget(timeline);
    connect(this, &MainWindow::animationSet, timeline, &Timeline::setAnimation);
    connect(this, &MainWindow::animationSet, frameEditor, &FrameEditor::setAnimation);
    connect(frameEditor, &FrameEditor::frameChanged, timeline, &Timeline::onFrameChanged);
    connect(timeline, &Timeline::selectionChanged, frameEditor, &FrameEditor::setSelection);
    connect(&_mainMenu, &MainMenu::copy, timeline, &Timeline::copyFrames);
    connect(&_mainMenu, &MainMenu::cut, timeline, &Timeline::cutFrames);
    connect(&_mainMenu, &MainMenu::paste, timeline, &Timeline::pasteFrames);

    connect(colorPick, &ColorPicker::sendColor, frameEditor, &FrameEditor::setPenColor);
}

MainWindow::~MainWindow()
{

}

// TODO: free the old animation after sending animationSet.
void MainWindow::setAnimation(Animation *animation) {
    _animation = animation;
    animationSet(animation);
}
