/* CONTRIBUTORS
 * Ben Merritt: Outlined initial signals and slots
 * Nick Valenti: Connected/wrote signals and slots, assisted building the GUI, wrote the change and update functions
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

    // Points to loaded animation from timeline.h
    Animation *animation;

    // Locally tracked variables
    QColor curCol;
    int curIndex;

public:
    enum ToolType {
        // TODO: Implement Selection
        // Pen and pick color handled by current colorpicker
        PEN,
        SELECTION,
        PICK_COLOR
    };
    FrameEditor(QPushButton *parent = 0);
    explicit FrameEditor(QWidget *parent = 0);
    void initializeLayout(QGridLayout *curLayout);


signals:
    // Signals timeline widget to redraw
    void frameChanged(int index);

public slots:
    // Changes actively displayed frame
    void setSelection(FrameSelection selection);
    // Resets pointer due to a change in *animation
    void setAnimation(Animation *newAnimation);
    void selectTool(ToolType tool);
    void setPenColor(const QColor& color);
    // Relative logic, for use by all cells
    void onCellClickEvent();
    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();
};

#endif // FRAMEEDITOR_H
