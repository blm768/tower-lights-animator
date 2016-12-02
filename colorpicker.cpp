#include "colorpicker.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <iostream>
#include <QLabel>
#include <QColorDialog>
#include <QTextStream>

ColorPicker::ColorPicker(QWidget *parent) : QWidget(parent)
{
    DefaultColors[0][0] = (QColor(Qt::blue));
    DefaultColors[0][1] = (QColor(Qt::red));
    DefaultColors[0][2] = (QColor(Qt::yellow));
    DefaultColors[1][0] = (QColor(255,165,0,255));
    DefaultColors[1][1] = (QColor(Qt::green));
    DefaultColors[1][2] = (QColor(255,0,255,255));
    DefaultColors[2][0] = (QColor(Qt::black));
    DefaultColors[2][1] = (QColor(Qt::white));
    DefaultColors[2][2] = (QColor(150,150,150,255));

    col.setRgb(0,0,0);
    QWidget* hozBox = new QWidget();
    QWidget* gridBox = new QWidget();
    QGridLayout *EditorLayout = new QGridLayout();
    QHBoxLayout *MainLayout = new QHBoxLayout(this);
    ColSel = new QLabel();

    hozBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    //hozBox->setLayout(MainLayout);
    this->setLayout(MainLayout);
    gridBox->setLayout(EditorLayout);

    ColSel->setFixedSize(80,80);




    ColSel->setStyleSheet("QLabel { background-color : black; }");
    EditorLayout->setSizeConstraint(QLayout::SetFixedSize);


    for (int i = 0; i < Height; i++){
        for (int j = 0; j < Width; j++){
            QPushButton *button = new QPushButton;
            connect(button, SIGNAL(clicked()), this, SLOT(setColor()));
            EditorLayout->addWidget(button,i,j);
        }
    }
    initializeLayout(EditorLayout);
    MainLayout->addWidget(gridBox);
    MainLayout->addWidget(ColSel);
    MainLayout->addStretch(1);




}

void ColorPicker::initializeLayout(QGridLayout *curLayout){

    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            QLayoutItem *layout = curLayout->itemAtPosition(i,j);
            QWidget *widget = layout->widget();
            QPushButton *current = qobject_cast<QPushButton*>(widget);

            current->setMaximumSize(QSize(27,27));
            current->setMinimumSize(QSize(27,27));

            QString css = "background-color: " + DefaultColors[i][j].name() + "; border: 2px solid #777777";
            if (i == Height-1 && j == Width-1){
                css.append("; font: bold 8px");
                current->setText("more");
            }
            current->setStyleSheet(css);
            current->setFlat(true);
            current->setAutoFillBackground(true);
        }
    }


}

 void ColorPicker::setColor(){

     QPushButton *current = qobject_cast<QPushButton*>(sender());
     QGridLayout *layout = dynamic_cast<QGridLayout*>(current->parentWidget()->layout());
     int index = layout->indexOf(current);
     int x, y, rs, cs;
     layout->getItemPosition(index, &x, &y, &rs, &cs);

     col = DefaultColors[x][y];

     if(x == 2 && y == 2) {
        ColDiag = new QColorDialog(this);

        ColDiag->open();
        connect(ColDiag, SIGNAL(colorSelected(QColor)),this, SLOT(diagOpt(QColor)));


     }
     colorChange();
     sendColor(col);


 }

 void ColorPicker::colorChange(){
     int r,g,b;
     col.getRgb(&r,&g,&b);

     QString red, green, blue, css;
     red = QString::number(r);
     blue = QString::number(b);
     green = QString::number(g);

     css = "QLabel { background-color : rgb(";
     css.append(red);
     css.append(",");
     css.append(green);
     css.append(",");
     css.append(blue);
     css.append("); }");

     ColSel->setStyleSheet(css);

 }

 void ColorPicker::diagOpt(QColor color){

     int r,g,b;
     color.getRgb(&r,&g,&b);
     col.setRgb(r,g,b);
     colorChange();
     sendColor(col);

 }
