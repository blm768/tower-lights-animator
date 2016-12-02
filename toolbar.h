#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QToolButton>

#include "frameeditor.h"

class Toolbar : public QWidget
{
    Q_OBJECT
public:
    explicit Toolbar(QWidget *parent = 0);

    QWidget *hozLayoutWidget;
    QHBoxLayout *hozLayout;
    QPushButton *pen;
    QPushButton *selector;
    QPushButton *colorPicker;

signals:
    void toolSelected(FrameEditor::ToolType tool);

private slots:
    void handlePen();
    void handleSelector();
    void handleColorPicker();
};

#endif // TOOLBAR_H
