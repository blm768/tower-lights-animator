#include "toolbar.h"
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <iostream>
using namespace std;
Toolbar::Toolbar(QWidget *parent) : QWidget(parent) {

//Create a horizontal group to add the buttons to
    hozLayoutWidget = new QWidget(parent);
    hozLayoutWidget-> setGeometry(QRect(375, 25, 240, 20));
    hozLayout = new QHBoxLayout(hozLayoutWidget);
    hozLayout->setObjectName(QStringLiteral("toolbar"));
    hozLayout->setContentsMargins(0,0,0,0);

//Create a button for each of the tooltypes
//Unimplemented features

    /*
    selector = new QPushButton(hozLayoutWidget);
    selector->setText("Select");
    selector->setObjectName(QStringLiteral("Select"));
    hozLayout->addWidget(selector);

    pen = new QPushButton(hozLayoutWidget);
    pen->setText("Pen");
    pen->setObjectName(QStringLiteral("Pen"));
    hozLayout->addWidget(pen);

    colorPicker = new QPushButton(hozLayoutWidget);
    colorPicker->setText("Pick Color");
    colorPicker->setObjectName(QStringLiteral("Pick Color"));
    hozLayout->addWidget(colorPicker);

    connect(pen, SIGNAL(clicked()), this, SLOT(handlePen()));
    connect(selector, SIGNAL(clicked()), this, SLOT(handleSelector()));
    connect(colorPicker, SIGNAL(clicked()), this, SLOT(handleColorPicker()));
    */
}

//Internal slots for each tool
void Toolbar::handlePen() {

    toolSelected(FrameEditor::ToolType::PEN);
}
void Toolbar::handleSelector() {

    toolSelected(FrameEditor::ToolType::SELECTION);
}
void Toolbar::handleColorPicker() {

    toolSelected(FrameEditor::ToolType::PICK_COLOR);
}
