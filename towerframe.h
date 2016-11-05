#ifndef TOWERFRAME_H
#define TOWERFRAME_H
#include <QColor>
#include <QTime>
#include <QList>

class TowerFrame
{
private:
    static const int FWIDTH = 12;
    static const int FHEIGHT = 20;

    struct Frame {
        QTime FDuration;
        // Does not requre dynamic allocation at this time, however allocation
        // preserved for possible future case in dynamic sized selections
        /*QColor **WorkArea = new QColor*[FWIDTH];
        Frame() {
            for (int i = 0; i < FWIDTH; i++) {
                WorkArea[i] = new QColor[FHEIGHT];
            }
        }*/
        QColor WorkArea[FWIDTH][FHEIGHT];
    };
    typedef Frame *frameptr;

    QTime TDuration;    // Stores duration of entire FrameList
    QList<frameptr> FrameList;

public:
    TowerFrame();


    void ColorCell(int Index, int x, int y, QColor Color);
    int AddFrame(QTime Duration);               // Add a frame with Duration to end
    int AddFrame(int Index);                    // Add a copy of frame at Index to end
    int GetDuration();

        // Awaiting Implementation
    int AddFrame(int Index, int Position);      // Add a copy of frame at Index to Position
    int AddFrame(QTime Duration, int Postition);// Add a new frame to Position with Duration
    void DeleteFrame(int Position);             // Deletes frame at Position

    int MoveFrame(int IndexFrom, int IndexTo);

        // Testing
    void PrintTower();

};

#endif // TOWERFRAME_H
