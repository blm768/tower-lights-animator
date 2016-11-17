#ifndef TOWERFRAME_H
#define TOWERFRAME_H
#include <QColor>
#include <QTime>
#include <QList>

#define FHEIGHT 20
#define FWIDTH 12


struct Frame
{
       QTime FDuration;
       QColor WorkArea[FHEIGHT][FWIDTH];

       // Returns the duration of the frame in milliseconds
       int toMsec()
       {
            return (QTime(0,0,0,0).msecsTo(FDuration));
       }
};
typedef Frame *frameptr;

class Animation
{
private:
    int FrameCount = 0;
    QTime TDuration;                    // Stores duration of entire FrameList
    QTime SanitizeTime(QTime InTime);   // Ensures InTime is valid and divisible by 25, returns a
                                        // value that is valid, rounds up

public:
<<<<<<< HEAD
=======
    struct Frame {
        QTime FDuration;
        QColor WorkArea[FHEIGHT][FWIDTH];

        // Returns the duration of the frame in milliseconds
        int toMsec()
        {
            return (QTime(0,0,0,0).msecsTo(FDuration));
        }
        QColor& cell(size_t width, size_t height);
    };
    typedef Frame *frameptr;
>>>>>>> 4c06a2c466aba78220968ecd325435cf0eb4b526

        // Stores the current frame in the animation
    frameptr currFrame;
    QList<frameptr> FrameList;

    Animation();

            // Sets cell [row][column] to Color in frame located at Index
            // returns 0 if Index, row, or column are out of bounds
            // If Color is not valid it sets it to Qt::black
    int ColorCell(int Index, int row, int column, QColor Color);

            // Returns the color of the cell in frame Index at row and column
            // returns Qt::black if any values are invalid
    QColor GetCellColor(int Index, int row, int column);

            // Does everything the previous one does excepts sets
            // the color of the currently selected frame in tan file
            // and no index cause we are going linearly
    int ColorCell(int row, int column, QColor Color);

            // Returns the length of the entire sequence in ms
    int GetDuration();

            // Sets the frame at Index's duration to Duration
            // returns 0 if Index is out of bounds
    int SetFrameDuration(QTime Duration, int Index);

            // Returns the duration of frame Index in ms
            // returns 0 if Index is out of bounds
    int GetFrameDuration(int Index);

    void CreateNewFrame();
            // Points the currFrame pointer at
            // a newly created frame for tan file

    int  AddColoredFrame(QTime pTime, QTime nTime);
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

            // Returns true if frame Index exists and row and column are
            // within the bounds of WorkArea, false otherwise
    bool IsValidFrameCell(int Index, int row, int column);

            // Returns true if frame Index exists, false otherwise
    bool IsValidFrame(int Index);

            // Returns true if row and column are within the bounds of
            // Workarea, false otherwise
    bool IsValidCell(int row, int column);


        // Testing only
    void PrintTower();

};

#endif // TOWERFRAME_H
