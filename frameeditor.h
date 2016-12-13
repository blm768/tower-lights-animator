/* CONTRIBUTORS
 * Ben Merritt: Outlined initial signals and slots
 */
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
private:
    QGridLayout *EditorLayout = new QGridLayout(this);
    Animation *animation;
    QColor curCol;
    int curIndex;

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
    void frameChanged(int index);

public slots:
    void setSelection(FrameSelection selection);
    void setAnimation(Animation *newAnimation);
    void selectTool(ToolType tool);
    void setPenColor(const QColor& color);
    void onCellClickEvent();
    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();
};

#endif // FRAMEEDITOR_H
