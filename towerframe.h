/* CONTRIBUTORS
 * Nick Valenti: wrote the towerframe class and did most of the non shift related updating and maintenance
 * Ben Merritt: added a couple of constants and the declaration of GetFrame()
 * Paden Rumsey: added a variety of frame adding methods (outside of shift) and implemented all of the shifting functions
 */
#ifndef TOWERFRAME_H
#define TOWERFRAME_H
#include <QColor>
#include <QTime>
#include <QList>

#define FHEIGHT 20
#define FWIDTH 12

#define FRAME_H_MARGIN 4
#define FRAME_V_MARGIN 5
#define FRAME_INNER_WIDTH 4
#define FRAME_INNER_HEIGHT 10

struct Frame
{
       int FDuration;
       QColor WorkArea[FHEIGHT][FWIDTH];

       // Returns the duration of the frame as QTime
       QTime toQTime()
       {
           return (QTime(0,0,0,0).addMSecs(FDuration));
       }
};
typedef Frame *frameptr;

class Animation
{
private:
    int TDuration;                      // Stores duration of entire FrameList
    int SanitizeTime(int OutTimeT);       // Ensures InTime is valid and divisible by 25, returns a
                                        // value that is valid, rounds up


public:

        // Stores the current frame in the animation

    QList<frameptr> FrameList;

    frameptr currFrame;
    Animation();
    ~Animation();

    /*!
     *\brief Frees Animation memory
     */
     void FreeMemory();

    /*!
     * \brief Gets a pointer to the raw frame at the given index
     */
    Frame* GetFrame(int index);

    /*!
     * \brief Inserts the given frame at the given index
     */

    int InsertFrame(int index, Frame* frame);

    /*!
     * \brief Returns the number of frames in the animation
     */
    int FrameCount() const;

    bool FrameSelected();

            /*!
             * \brief Points currFrame to the Animation Frame at Index
             */
    int SelectFrame(int Index);

            /*!
             * \brief Sets cell [row][column] to Color in currFrame
             */
    int SetSelectedColor(int row, int column, QColor Color);

            /*!
             * \brief Returns the color of selected frame at cell [row][column]
             */
    QColor GetSelectedColor(int row, int column);

            /*!
             * \brief Sets cell [row][column] to Color in frame located at Index
             * returns 0 if Index, row, or column are out of bounds
             * If Color is not valid it sets it to Qt::black
             */
    int SetCellColor(int Index, int row, int column, QColor Color);

            /*!
             * \brief Returns the color of the cell in frame Index at row and column
             * returns Qt::black if any values are invalid
             */
    QColor GetCellColor(int Index, int row, int column);

            /*!
             * \brief Returns the color of the cell in frame Index at row and column
             * Does everything the previous one does excepts sets
             * the color of the currently selected frame in tan file
             * and no index cause we are going linearly
             */
    int ColorCell(int row, int column, QColor Color);

            /*!
             * \brief Returns the length of the entire sequence in ms
             */
    int GetDuration();

    // removed: no longer store duration as QTime
    // QTime GetDuration();

            /*!
             * \brief Sets the frame at Index's duration to Duration
             * returns 0 if Index is out of bounds
             */
    int SetFrameDuration(int Duration, int Index);

            /*!
             * \brief Returns the duration of frame Index in ms
             * returns 0 if Index is out of bounds
             */
    int GetFrameDuration(int Index);

    void CreateNewFrame();
            // Points the currFrame pointer at
            // a newly created frame for tan file

    int  AddColoredFrame(QTime pTime, QTime nTime);
            // Used to add a frame thats been created
            // from the tan file to a tower animation

            /*!
             * \brief Add a frame with Duration to end
             */
    void AddFrame(int Duration);

            /*!
             * \brief Add a copy of frame at Index to end
             * returns 0 if Index is out of bounds
             */
    int CopyFrame(int Index);


    // If Position is outside of the bounds of the List it will be
    // appended or prepended automatically
            /*!
             * \brief Add a new frame to Position with Duration
             */
    void AddFrame(int Duration, int Position);

            /*!
             * \brief Add a copy of frame at Index to Position
             * return 0 if Index is out of bounds
             */
    int CopyFrame(int Index, int Position);

            /*!
             * /brief Deletes frame at Position, returns 0 if position is out of bounds
             */
    int DeleteFrame(int Position);

            //Shift the frame up and to the left
    int ShiftFrameLU(int Index, int Position);

           //Shift the frame to the left
    int ShiftFrameL(int Index, int Position);

           //Shift the frame down and to the left
    int ShiftFrameLD(int Index, int Position);

          //Shift the frame up
    int ShiftFrameUp(int Index, int Position);

          //Shift the frame down
    int ShiftFrameDown(int Index, int Position);

           //Shift the frame up and to the right
    int ShiftFrameRU(int Index, int Position);

           //Shift the frame to the right
    int ShiftFrameR(int Index, int Position);

           //Shift the frame down and to the right
    int ShiftFrameRD(int Index, int Position);

            /*!
             * \brief Relocates frame at IndexFrom to before IndexTo
             * Returns 0 if either index is outside of list bounds
             */
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
