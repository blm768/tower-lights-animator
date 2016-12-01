#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QGridLayout>
#include <QColor>
#include <QLabel>
#include <QColorDialog>

class ColorPicker : public QWidget
{
    Q_OBJECT
private:
    QColor col;
    QLabel *ColSel;
    QColorDialog *ColDiag;
    static const int Height = 3;
    static const int Width = 3;
    QColor DefaultColors[Width][Height];

public:
    explicit ColorPicker(QWidget *parent = 0);
    void initializeLayout(QGridLayout *curLayout);


signals:
    void colorSet();
    void sendColor(const QColor color);


public slots:
    void setColor();
    void colorChange();
    void diagOpt(QColor color);

};

#endif // COLORPICKER_H
