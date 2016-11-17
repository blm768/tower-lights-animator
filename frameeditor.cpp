#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include "frameeditor.h"

#define cellsize 27
#define cellspace 4

/*
 * Clickable labels adapted from
 * https://wiki.qt.io/Clickable_QLabel
 */

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}

FrameEditor::FrameEditor(QWidget *parent) : QWidget(parent)
{
/*
    This is an example of a frame, the end result needs to be four QFrame::Line objects bordering our central area

    QFrame *border = new QFrame(parent);
    border->setFrameStyle(QFrame::Box);
    //border->setFrameShadow(QFrame::Raised);
    //border->setAutoFillBackground(true);

    border->setFixedSize((cellsize * 4) + (cellspace * 4),(cellsize * 10) + (cellspace * 10));
    border->setLineWidth(3);
    border->move((cellsize * 4) + (cellspace * 4.5)+5,(cellsize * 5) + (cellspace * 4.5) + 29);
    QPalette pal;
    pal.setColor(QPalette::Foreground, Qt::black);
    border->setPalette(pal);
*/

    QGridLayout *EditorLayout = new QGridLayout(this);
    EditorLayout->setSizeConstraint(QLayout::SetFixedSize);

    for (int i = 0; i < FHEIGHT; i++){
        for (int j = 0; j < FWIDTH; j++){
            QPushButton *button = new QPushButton;
            connect(button, SIGNAL(clicked()), this, SLOT(onCellClickEvent()));
            EditorLayout->addWidget(button,i,j);
        }
    }
    EditorLayout->setSpacing(cellspace);
    initializeLayout(EditorLayout);

    setLayout(EditorLayout);

    show();
}

void FrameEditor::initializeLayout(QGridLayout *curLayout)
{
    for (int i = 0; i < FHEIGHT; i++)
    {
        for (int j = 0; j < FWIDTH; j++)
        {

            QLayoutItem *layout = curLayout->itemAtPosition(i,j);
            QWidget *widget = layout->widget();
            QPushButton *current = qobject_cast<QPushButton*>(widget);

            //Palette is no longer used, stylesheet is now used

            //QPalette pal;
            //pal.setColor(QPalette::Button, QColor(Qt::black));
            //current->setFrameShape(QFrame::WinPanel);
            //current->setFrameShadow(QFrame::Sunken);
            //current->setPalette(pal);

            current->setMaximumSize(QSize(cellsize,cellsize));
            current->setMinimumSize(QSize(cellsize,cellsize));

            QString css = "background-color: #000000; border: 2px solid #777777";
            current->setStyleSheet(css);

            current->setFlat(true);
            current->setAutoFillBackground(true);

        }
    }
}

void FrameEditor::onCellClickEvent()
{
    QPushButton *current = qobject_cast<QPushButton*>(sender());

    //QPalette pal = current->palette();
    //pal.setColor(QPalette::Button, QColor(Qt::green));
    //current->setPalette(pal);

    int x = current->x();
    int y = current->y();

    // This sets the text of the buttons to their x and y coordinate, note it's not their index
    current->setText(QString::number(x) + "," + QString::number(y));

    QString back = "00ffff";
    //QString border = "000000";

    QString css = "background-color: #" + back + "; border: 1px solid #777777";
    current->setStyleSheet(css);
}

void FrameEditor::setSelection(FrameSelection selection)
{
    if(selection.length() == 1)
    {
        // TODO: switch frames
    }
    else
    {
        // We can't edit 0 or multiple frames.
        // TODO: disable this widget?
    }
}

void FrameEditor::selectTool(ToolType tool)
{

}

void FrameEditor::setPenColor(const QColor& color)
{

}

void FrameEditor::onCopyEvent()
{

}

void FrameEditor::onCutEvent()
{

}

void FrameEditor::onPasteEvent()
{

}
