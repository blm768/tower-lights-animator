#include "colorpicker.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <iostream>
#include <QLabel>
#include <QColorDialog>
#include <QTextStream>
#include <QLayout>

ColorPicker::ColorPicker(QWidget *parent) : QWidget(parent)
{
    col.setRgb(0,0,0);
    QHBoxLayout *MainLayout = new QHBoxLayout(this);

    this->setLayout(MainLayout);

    QColorDialog *colPicker = new QColorDialog(parent);
    connect(colPicker, SIGNAL(currentColorChanged(QColor)), this, SLOT(setColor(QColor)));
    colPicker->setWindowFlags(Qt::Widget);
    colPicker->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);

    MainLayout->addWidget(colPicker);
    MainLayout->addStretch(1);

}

void ColorPicker::setColor(QColor color) {
    col = color;
    sendColor(col);
}
