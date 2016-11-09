#ifndef FRAMEEDITOR_H
#define FRAMEEDITOR_H

#include <QWidget>

#include "towerframe.h"

class FrameEditor : public QWidget
{
    Q_OBJECT
public:
    enum ToolType {
        PEN,
        SELECTION,
        PICK_COLOR
    };

    explicit FrameEditor(QWidget *parent = 0);

signals:
    void frameChanged(Animation::Frame* frame);

public slots:
    void selectionChanged(QList<Animation::Frame*> selection);
    void selectTool(ToolType tool);
    void setPenColor(const QColor& color);
    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();
};

#endif // FRAMEEDITOR_H
