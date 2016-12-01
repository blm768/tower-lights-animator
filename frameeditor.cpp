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
#define cellspace 1

FrameEditor::FrameEditor(QWidget *parent) : QWidget(parent)
{

    //QGridLayout *EditorLayout = new QGridLayout(this);
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

    QString border = "777777";
    if (row > 4 && row < FHEIGHT - 5){
        if (column > 3 && column < FWIDTH - 4){
            border = "ff0000";
        }
    }

    QString back = "00ffff";

    //current->setText(QString("%1,%2").arg(row).arg(column));

    QString css = "background-color: #" + back + "; border: 2px solid #" + border;
    current->setStyleSheet(css);
}

void FrameEditor::setSelection(FrameSelection selection)
{
    if(selection.length() == 1)
    {
        QString rgb, css, border;
        int frame;
        for (int i = 0; i < FHEIGHT; i++){
            for (int j = 0; j < FWIDTH; j++){

                QLayoutItem *layout = EditorLayout->itemAtPosition(i,j);
                QWidget *widget = layout->widget();
                QPushButton *current = qobject_cast<QPushButton*>(widget);

                frame = selection.start;

                rgb = selection.animation->GetCellColor(frame,i,j).name();
                border = "777777";
                if (i > 4 && i < FHEIGHT - 5){
                    if (j > 3 && j < FWIDTH - 4){

                        border = "ff0000";
                    }
                }
                css = "background-color: " + rgb + "; border: 2px solid #" + border;
                current->setStyleSheet(css);
            }
        }
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
