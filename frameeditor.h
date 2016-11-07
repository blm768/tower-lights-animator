#ifndef FRAMEEDITOR_H
#define FRAMEEDITOR_H

#include <QWidget>

#include "frame.h"

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
    void frameChanged(Frame* frame);

public slots:
    void selectionChanged(QList<Frame*> selection);
    void selectTool(ToolType tool);
    void setPenColor(const QColor& color);
    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();
};

#endif // FRAMEEDITOR_H
