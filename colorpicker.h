#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = 0);

signals:
    void colorSet(QColor color);


public slots:
    void setColor(QColor color);

};

#endif // COLORPICKER_H
