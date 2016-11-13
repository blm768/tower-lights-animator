#include <QGridLayout>
#include <QPushButton>
#include "frameeditor.h"

FrameEditor::FrameEditor(QWidget *parent) : QWidget(parent)
{
    QGridLayout *EditorLayout = new QGridLayout(this);
    EditorLayout->setSizeConstraint(QLayout::SetFixedSize);

    for (int i = 0; i < FHEIGHT; i++){
        for (int j = 0; j < FWIDTH; j++){
            QPushButton *button = new QPushButton;
            connect(button, SIGNAL(clicked()), this, SLOT(onCellClickEvent()));
            EditorLayout->addWidget(button,i,j);
        }
    }
    EditorLayout->setSpacing(2);
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

            QPalette pal = current->palette();
            pal.setColor(QPalette::Button, QColor(Qt::black));
            /*
            if (i > 4 && i < 12 && j > 5 && j < 15)
            {
                pal.setColor(QPalette::Window, QColor(Qt::red));
            }
            */
            current->setMaximumSize(QSize(25,25));
            current->setMinimumSize(QSize(25,25));

            current->setFlat(true);
            current->setAutoFillBackground(true);
            current->setPalette(pal);
        }
    }
}

void FrameEditor::onCellClickEvent()
{
    QPushButton *current = qobject_cast<QPushButton*>(sender());
    QPalette pal = current->palette();
    pal.setColor(QPalette::Button, QColor(Qt::green));
    current->setPalette(pal);

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
