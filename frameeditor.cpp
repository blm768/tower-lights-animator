#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QPointer>
#include <QEvent>
#include <QPen>
#include "frameeditor.h"

#define cellsize 27
#define cellspace 4

/*
 * Clickable labels adapted from
 * https://wiki.qt.io/Clickable_QLabel
 */

class Overlay : public QWidget {
public:
    Overlay(QWidget * parent = 0) : QWidget(parent) {
        setAttribute(Qt::WA_NoSystemBackground);
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }
protected:
    void paintEvent(QPaintEvent *) {
        QPainter p(this);
        QPen pen(Qt::magenta);
        pen.setWidth(5);
        pen.setStyle(Qt::DotLine);
        p.setPen(pen);
        p.drawRect(rect());
    }
};


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
    curCol.setRgb(0,0,0);
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



    for(int i = 5; i < FHEIGHT - 5; i++){
        for(int j = 4; j < FWIDTH - 4; j++){
            Overlay *m = new Overlay;
            EditorLayout->addWidget(m,i,j);
        }
    }


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

            current->setMaximumSize(QSize(cellsize,cellsize));
            current->setMinimumSize(QSize(cellsize,cellsize));
            QString border = "777777";
            if (i > 4 && i < FHEIGHT - 5){
                if (j > 3 && j < FWIDTH - 4){
                    border = "ff0000";
                }
            }
            QString css = "background-color: #000000; border: 2px solid #" + border;
            current->setStyleSheet(css);

            current->setFlat(true);
            current->setAutoFillBackground(true);

        }
    }

}

void FrameEditor::onCellClickEvent()
{
    QPushButton *current = qobject_cast<QPushButton*>(sender());

    QGridLayout *layout = dynamic_cast<QGridLayout*>(current->parentWidget()->layout());
    int index = layout->indexOf(current);
    int row, column, rs, cs;
    layout->getItemPosition(index, &row, &column, &rs, &cs);


    int r,g,b;
    curCol.getRgb(&r,&g,&b);

    QString red, green, blue, css;
    red = QString::number(r);
    blue = QString::number(b);
    green = QString::number(g);

    css = "background-color : rgb(";
    css.append(red);
    css.append(",");
    css.append(green);
    css.append(",");
    css.append(blue);
    css.append("); ");
    



    QString border = "777777";
    if (row > 4 && row < FHEIGHT - 5){
        if (column > 3 && column < FWIDTH - 4){
            border = "ff0000";
        }
    }
    css.append("border: 1px solid #" + border);

    
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
int r,g,b;
color.getRgb(&r,&g,&b);
curCol.setRgb(r,g,b);

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
