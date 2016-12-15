/* CONTRIBUTORS
 * Nick Valenti: updated existing solution to use QColorDialog
 * Dylan Carlson: Worked on original Color Picker, and connecting the Color Picker to the Frame Editor
 */

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

public:
    explicit ColorPicker(QWidget *parent = 0);
    void initializeLayout(QGridLayout *curLayout);


signals:
    void colorSet();
    void sendColor(const QColor color);


public slots:
    void setColor(QColor color);

};

#endif // COLORPICKER_H
