/* CONTRIBUTORS
 * Nick Valenti: Wrote playback.h and playback.cpp
 */

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QList>
#include <QGridLayout>
#include <QCloseEvent>
#include <QApplication>
#include "towerframe.h"


class Playback : public QWidget
{
    Q_OBJECT
private:
    // TODO: clean up this qgridlayout after media player is added
    QGridLayout *PBLayout = new QGridLayout(this);
    Animation *animation;

    // Contains list of timestamps corresponding to frames
    // WARNING: will segfault if not kept updated
    // TODO: pros/cons of adding timestamps to animation class?
    QList<QTime> timestamps;
    QTimer *timer = new QTimer(this);

    int currentindex;
    int play;

    void setColor(int x, int y, QString color);

private slots:
    // Meant to be used by QSlider, will be inconsistant if generalized
    void setTime(int ms);
    void onPlayEvent();
    // Updates frame when timer fires
    void incTime();

public slots:
    void setAnimation(Animation* anim);

signals:
    // Communicates with mainwindow to force only one playback window open,
    // can avoid updating
    void closeEvent (QCloseEvent *event);

public:
    Playback(QWidget *parent = 0, Animation *anim = NULL);
    // on main window animation update
    // also recalculates timestamps
    // TODO: update on all changes and not just high priority segfault avoidance?
    void refreshAnim();

};

#endif // PLAYBACK_H
