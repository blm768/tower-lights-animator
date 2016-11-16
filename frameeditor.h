#ifndef FRAMEEDITOR_H
#define FRAMEEDITOR_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
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
    FrameEditor(QPushButton *parent = 0);
    explicit FrameEditor(QWidget *parent = 0);
    void initializeLayout(QGridLayout *curLayout);

signals:
    void frameChanged(Frame* frame);

public slots:
    void selectionChanged(QList<Frame*> selection);
    void selectTool(ToolType tool);
    void setPenColor(const QColor& color);
    void onCellClickEvent();
    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();
};

#endif // FRAMEEDITOR_H
