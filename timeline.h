#ifndef TIMELINE_H
#define TIMELINE_H

#include <QWidget>

#include "towerframe.h"

class Timeline : public QWidget
{
    Q_OBJECT
public:
    explicit Timeline(QWidget *parent = 0);

signals:
    void selectionChanged(QList<Animation::Frame*> frames);

public slots:
    void animationLoaded(QList<Animation::Frame*> frames);
    void onCopyEvent();
    void onCutEvent();
    void onPasteEvent();
};

#endif // TIMELINE_H
