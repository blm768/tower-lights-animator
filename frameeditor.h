#ifndef FRAMEEDITOR_H
#define FRAMEEDITOR_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "towerframe.h"

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel( const QString& text="", QWidget* parent=0 );
    ~ClickableLabel();
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);
};

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
