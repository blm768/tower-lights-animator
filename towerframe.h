#ifndef TOWERFRAME_H
#define TOWERFRAME_H
#include <QTGui/QColor>
#include <QTime>
#include <QList>

class TowerFrame
{
private:
    static const int FWIDTH = 12;
    static const int FHEIGHT = 20;

    struct Frame {
        QTime FDuration;
        QColor **WorkArea = new QColor*[FWIDTH];
        Frame() {
            for (int i = 0; i < FWIDTH; i++) {
                WorkArea[i] = new QColor[FHEIGHT];
            }
        }
    };
    typedef Frame *frameptr;

    QTime TDuration;    // Stores duration of entire FrameList
    QList<frameptr> FrameList;

public:
    TowerFrame();

    int AddFrame(QTime Duration);   // Adds a frame to the end of FrameList with passed Duration
    int AddFrame(int Index);        // Adds a copy of frame at index Index to end of FrameList
    int InsertFrame(int IndexFrom, int IndexTo);
    void ColorCell(int Index, int x, int y, QColor Color);

    void PrintTower();
    int GetDuration();

};

#endif // TOWERFRAME_H
