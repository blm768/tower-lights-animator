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

            if(i == 0 && j == 0){
            QString css = "background-color: blue; border: 2px solid #777777";
            current->setStyleSheet(css);
            }

            if(i == 0 && j == 1){
            QString css = "background-color: red; border: 2px solid #777777";
            current->setStyleSheet(css);
            }

            if(i == 0 && j == 2){
            QString css = "background-color: yellow; border: 2px solid #777777";
            current->setStyleSheet(css);
            }

            if(i == 1 && j == 0){
            QString css = "background-color: orange; border: 2px solid #777777";
            current->setStyleSheet(css);
            }

            if(i == 1 && j == 1){
            QString css = "background-color: green; border: 2px solid #777777";
            current->setStyleSheet(css);
            }

            if(i == 1 && j == 2){
            QString css = "background-color: purple; border: 2px solid #777777";
            current->setStyleSheet(css);
            }

            if(i == 2 && j == 0){
            QString css = "background-color: black; border: 2px solid #777777";
            current->setStyleSheet(css);
            }

            if(i == 2 && j == 1){
            QString css = "background-color: white; border: 2px solid #777777";
            current->setStyleSheet(css);
            }

            if(i == 2 && j == 2){
            QString css = "background-color: grey; border: 2px solid #777777; font: bold 8px";
            current->setStyleSheet(css);
            current->setText("more");
            }


            current->setFlat(true);
            current->setAutoFillBackground(true);

        }
    }


}

 void ColorPicker::setColor(){

     QPushButton *current = qobject_cast<QPushButton*>(sender());
     int x = current->x();
     int y = current->y();

     if(x == 12 && y == 12) {
       col.setRgb(0,0,255); //blue

     }
     if(x == 53 && y == 12){
       col.setRgb(255,0,0); //red

     }
     if(x == 94 && y == 12){
       col.setRgb(255,255,0); //yellow

     }
     if(x == 12 && y == 53){
       col.setRgb(255,165,0); //orange

     }
     if(x == 53 && y == 53){
       col.setRgb(0,255,0); //Green

     }
     if(x == 94 && y == 53){
       col.setRgb(255,0,255); //Purple

     }
     if(x == 12 && y == 94){
        col.setRgb(0,0,0); //Black

     }
     if(x == 53 && y == 94){
        col.setRgb(255,255,255); //White

     }
     if(x == 94 && y == 94) {
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

 }

