#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

#include "frameeditor.h"

class Toolbar : public QWidget
{
    Q_OBJECT
public:
    explicit Toolbar(QWidget *parent = 0);

signals:
    void toolSelected(FrameEditor::ToolType tool);

public slots:
};

#endif // TOOLBAR_H
