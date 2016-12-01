#ifndef FRAMEEDITOR_H
#define FRAMEEDITOR_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLine>
#include <QPainter>


#include "towerframe.h"
#include "timeline.h"

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

    QGridLayout *EditorLayout = new QGridLayout(this);

signals:
    void frameChanged(int index);

public slots:
    void setSelection(FrameSelection selection);
    //void FrameEditor::setSelection(Frame *WorkFrame);
    void selectTool(ToolType tool);
    void setPenColor(const QColor& color);
    void onCellClickEvent();
    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();
};

#endif // FRAMEEDITOR_H
