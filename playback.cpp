#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QSlider>
#include <QApplication>
#include <QCloseEvent>
#include "playback.h"
#include "frameeditor.h"

#define cellsize 50
#define cellspace 4

// TODO: these defines must be migrated to the animation class in towerframe.h
#define hoffset 5
#define voffset 4
#define TWIDTH 4
#define THEIGHT 10

Playback::Playback(QWidget *parent, Animation *anim)
{
    play = 0;

    // TODO: remove magic numbers
    // refresh rate 25ms matches tower
    timer->setInterval(25);
    connect(timer, SIGNAL(timeout()), this, SLOT(incTime()));

    currentindex = 0;
    animation = anim;

    if (animation != NULL) {

        for (int i = 0; i < THEIGHT; i++){
            for (int j = 0; j < TWIDTH; j++){
                QLabel *label = new QLabel;
                PBLayout->addWidget(label,i,j+2);
            }
        }
        PBLayout->setSpacing(cellspace);

        for (int i = 0; i < THEIGHT; i++)
        {
            for (int j = 0; j < TWIDTH; j++)
            {
                QLayoutItem *layout = PBLayout->itemAtPosition(i,j+2);
                QWidget *widget = layout->widget();
                QLabel *current = qobject_cast<QLabel*>(widget);

                current->setMaximumSize(QSize(cellsize,cellsize));
                current->setMinimumSize(QSize(cellsize,cellsize));

                QString color = animation->GetSelectedColor(i+hoffset,j+voffset).name();

                setColor(i, j, color);
                current->setAutoFillBackground(true);
            }
        }

        QSlider *progress = new QSlider(Qt::Horizontal);
        progress->setTickPosition(QSlider::TicksAbove);
        progress->setTickInterval(1000);

        // TODO: magic numbers, cleanup of qgridlayout coordinates
        progress->setSingleStep(25);
        progress->setPageStep(25);
        PBLayout->addWidget(progress, THEIGHT+3,2,1,4);

        QLabel *time = new QLabel(QTime(0,0,0,0).toString("mm:ss.zzz"));
        PBLayout->addWidget(time,THEIGHT+2,3,1,2);

        QPushButton *playpause = new QPushButton(tr("Play"));
        playpause->setFixedWidth(50);
        connect(playpause, SIGNAL(clicked()), this, SLOT(onPlayEvent()));
        PBLayout->addWidget(playpause,THEIGHT+2,2,1,2);

        connect(progress, &QSlider::valueChanged, this, &Playback::setTime);

        setLayout(PBLayout);
    }
    refreshAnim();
}

void Playback::onPlayEvent()
{
    if (play == 0)
    {
        // begin playing
        QLayoutItem *layout = PBLayout->itemAtPosition(THEIGHT+3,2);
        QWidget *widget = layout->widget();
        QSlider *current = qobject_cast<QSlider*>(widget);

        if (current->value() == animation->GetDuration())
        {
            current->setValue(0);
            setTime(0);
        }

        play = 1;
        timer->start();
    }
    else
    {
        // pause playing
        play = 0;
        timer->stop();
    }
}

void Playback::incTime()
{
    QLayoutItem *layout = PBLayout->itemAtPosition(THEIGHT+3,2);
    QWidget *widget = layout->widget();
    QSlider *current = qobject_cast<QSlider*>(widget);

    if (current->value() + 25 < animation->GetDuration()) {
        current->setValue(current->value() + 25);
        setTime(current->value());
    } else {
        timer->stop();
        current->setValue(animation->GetDuration());
        setTime(current->value());
        play = 0;
    }

}

void Playback::setColor(int x, int y, QString color)
{
    QLayoutItem *layout = PBLayout->itemAtPosition(x,y+2);
    QWidget *widget = layout->widget();
    QLabel *current = qobject_cast<QLabel*>(widget);

    QString css = "background-color: " + color + "; border: 2px solid #777777";
    current->setStyleSheet(css);
}

void Playback::setTime(int ms)
{
    QLayoutItem *layout = PBLayout->itemAtPosition(THEIGHT+2,3);
    QWidget *widget = layout->widget();
    QLabel *time = qobject_cast<QLabel*>(widget);

    QTime temp = QTime(0,0,0,0).addMSecs(ms);
    time->setText(temp.toString("mm:ss.zzz"));

    int index;

    // only redraw qlabels if index changes
    // because of reliance on stylesheets labels will be redrawn
    // any time a change in stylesheet is made
    // TODO: further optimize by changing to pallet implementation
    for (int i = 0; i < animation->FrameCount(); i++)
    {
        if (temp >= timestamps.at(i))
        {
            index = i;
        }
        else
        {
            i = animation->FrameCount();
        }
    }
    if (index != currentindex){
        for (int i = 0; i < THEIGHT; i++)
        {
            for (int j = 0; j < TWIDTH; j++)
            {
                setColor(i, j, animation->FrameList.at(index)->WorkArea[i+hoffset][j+voffset].name());
            }
        }
        currentindex = index;
    }
}

void Playback::setAnimation(Animation* anim)
{
    animation = anim;
    refreshAnim();
}

void Playback::refreshAnim()
{
    timestamps.clear();
    QTime temp = QTime(0,0,0,0);
    if (animation != NULL)
    {
        for (int i = 0; i < animation->FrameCount(); i++)
        {
            timestamps.append(temp);
            temp = temp.addMSecs(animation->FrameList.at(i)->FDuration);
        }
        QLayoutItem *layout = PBLayout->itemAtPosition(THEIGHT+3,2);
        QWidget *widget = layout->widget();
        QSlider *progress = qobject_cast<QSlider*>(widget);

        progress->setMaximum(animation->GetDuration());
    }
    setTime(0);
}
