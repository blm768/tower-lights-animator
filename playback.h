#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QList>
#include <QGridLayout>
#include <QApplication>
#include "towerframe.h"


class Playback : public QWidget
{
    Q_OBJECT
private:
    QGridLayout *PBLayout = new QGridLayout(this);
    Animation *animation;
    QList<QTime> timestamps;
    QTimer *timer = new QTimer(this);

    int currentindex;
    int play;

    void setColor(int x, int y, QString color);

private slots:
    void setTime(int ms);
    void onPlayEvent();
    void incTime();

public:
    Playback(Animation *anim);
    void refreshAnim();

};

#endif // PLAYBACK_H
