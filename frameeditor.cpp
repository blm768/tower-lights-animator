#include <QGridLayout>
#include <QPushButton>
#include "frameeditor.h"

FrameEditor::FrameEditor(QWidget *parent) : QWidget(parent)
{
    QGridLayout *EditorLayout = new QGridLayout(this);

    for (int i = 0; i < FHEIGHT; i++){
        for (int j = 0; j < FWIDTH; j++){
            EditorLayout->addWidget(new QPushButton,i,j,1,1);

        }
    }
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
            QPushButton *current = dynamic_cast<QPushButton*>(widget);

            QPalette pal = current->palette();
            pal.setColor(QPalette::Button, QColor(Qt::black));

            current->setMaximumSize(QSize(25,25));
            current->setMinimumSize(QSize(25,25));

            current->setFlat(true);
            current->setAutoFillBackground(true);
            current->setPalette(pal);
        }
    }
}

void FrameEditor::ChangeCellColor()
{

}

void FrameEditor::selectionChanged(QList<Animation::Frame*> selection)
{

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
