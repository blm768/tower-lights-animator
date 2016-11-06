#ifndef TOWERFRAME_H
#define TOWERFRAME_H
#include <QColor>
#include <QTime>
#include <QList>

class TowerFrame
{
private:
    static const int FHEIGHT = 20;
    static const int FWIDTH = 12;

    struct Frame {
        QTime FDuration;
        // Does not requre dynamic allocation at this time
        // allocation preserved for possible future use with selections
        /*QColor **WorkArea = new QColor*[FHEIGHT];
        Frame() {
            for (int i = 0; i < FHEIGHT; i++) {
                WorkArea[i] = new QColor[FWIDTH];
            }
        }*/
        QColor WorkArea[FHEIGHT][FWIDTH];
    };
    typedef Frame *frameptr;

    frameptr currFrame;                 // Stores the current frame in the animation

    QTime TDuration;                    // Stores duration of entire FrameList
    QList<frameptr> FrameList;

    QTime SanitizeTime(QTime InTime);   // Ensures InTime is valid and divisible by 25, returns a
                                        // value that is valid, rounds up

public:
    TowerFrame();

            // Sets cell [row][column] to Color in frame located at Index
            // returns 0 if Index, row, or column are out of bounds
            // If Color is not valid it sets it to Qt::black
    int ColorCell(int Index, int row, int column, QColor Color);

            // Returns the length of the entire sequence in ms
    int GetDuration();

            // Sets the frame at Index's duration to Duration
            // returns 0 if Index is out of bounds
    int SetFrameDuration(QTime Duration, int Index);

            // Returns the duration of frame Index in ms
            // returns -1 if Index is out of bounds
    int GetFrameDuration(int Index);

    void CreateNewFrame();
            // Points the currFrame pointer at
            // a newly created frame for tan file

    void AddFrame(Frame newFrame, int Position);
            // Used to add a frame thats been created
            // from the tan file to a tower animation

            // Add a frame with Duration to end
    void AddFrame(QTime Duration);

            // Add a copy of frame at Index to end
            // returns 0 if Index is out of bounds
    int AddFrame(int Index);


    // If Position is outside of the bounds of the List it will be
    // appended or prepended automatically
            // Add a new frame to Position with Duration
    void AddFrame(QTime Duration, int Position);

            // Add a copy of frame at Index to Position
            // return 0 if Index is out of bounds
    int AddFrame(int Index, int Position);

            // Deletes frame at Position, returns 0 if position is out of bounds
    int DeleteFrame(int Position);

            // Relocates frame at IndexFrom to before IndexTo
            // Returns 0 if either index is outside of list bounds
    int MoveFrame(int IndexFrom, int IndexTo);

        // Testing only
    void PrintTower();

};

#endif // TOWERFRAME_H
