#include <QColor>
#include <QTime>
#include <QList>
#include <QString>
#include <QDebug>
#include <iostream>
#include "towerframe.h"


TowerFrame::TowerFrame()
{
    TDuration.setHMS(0, 0, 0, 0);
}

    /*
     * SanitizeTime should always return a value that is:
     * 1) isValid() = true (this means nonnegative and formatted correctly)
     * 2) is divisible by 25 ms (with the lowest accepted value being 25)
     * 3) rounded up to the nearest number that is divisible by 25 if it fails
     *    the second check, or just set to 25ms if it fails the first
     *
     * I tried to write it to quietly try to fix errors and return a valid time
     * as opposed to just returning error messages here
     */
QTime TowerFrame::SanitizeTime(QTime InTime)
{
    QTime OutTime = InTime;
    int MinMS = 25;
    if (!OutTime.isValid() || OutTime < QTime(0,0,0,MinMS))
    {
        OutTime = QTime(0,0,0,MinMS);
    }
    else
    {
        int remainder = (OutTime.msec() % MinMS);
        if (remainder != 0)
        {
            OutTime = OutTime.addMSecs(MinMS - remainder);
        }
    }
    return OutTime;
}

int TowerFrame::GetFrameDuration(int Index)
{
    if (Index < 0 || Index > FrameList.count())
    {
        // Index out of bounds
        return -1;
    }
    else
    {
        return (QTime(0,0,0,0).msecsTo(FrameList.at(Index)->FDuration));
    }
}

void TowerFrame::CreateNewFrame()
{
    currFrame = new Frame;

    for (int i = 0; i < FHEIGHT; i++)
    {
        for (int j = 0; j < FWIDTH; j++)
        {
            currFrame->WorkArea[i][j] = QColor(Qt::black);
        }
    }
}

int TowerFrame::AddColoredFrame(QTime pTime, QTime nTime)
{
    if(currFrame != NULL)
    {
        /* I do not personally believe that these checks should go here. I'm fairly
         * certain that Nick's Sanitize time function does some of these things
         * but not all. So I need someone to write/modify Nick's function to make
         * sure all these checks happen
         * 1) The duration is not negative.
         * 2) Is greater than 25 ms
         * 3) Is in a valid format
         * - Paden
         *
         */

        currFrame->FDuration = currFrame->FDuration.addMSecs(pTime.msecsTo(nTime));
        currFrame->FDuration = SanitizeTime(currFrame->FDuration);
/*
 *   I adjusted SanitizeTime to no longer return 0 ms, it now works properly in all cases
 *   -Nick
 */
        /*
        if(!currFrame->FDuration.isValid())
        {
            std::cout << "Error - timestamp in wrong format";
            return 0;
        }
        if(currFrame->FDuration <= QTime(0,0,0,0))
        {
            std::cout << "Error - previous timestamp greater than current timestamp or has no duration";
            return 0;
        }
        if(currFrame->FDuration < QTime(0,0,0,25))
        {
            std::cout << "Error - Timestamp does not meet minumum requirement";
            return 0;
        }
        */

        FrameList.append(currFrame);
        FrameCount++;
        TDuration = TDuration.addMSecs(pTime.msecsTo(nTime));
        return 1;
    }
    else
        return 0;
}

void TowerFrame::AddFrame(QTime Duration)
{
    frameptr n = new Frame;
    n->FDuration = SanitizeTime(Duration);

    for (int i = 0; i < FHEIGHT; i++)
    {
        for (int j = 0; j < FWIDTH; j++)
        {
            n->WorkArea[i][j] = QColor(Qt::black);
        }
    }

    FrameList.append(n);
    FrameCount++;
    TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(n->FDuration));
}

int TowerFrame::AddFrame(int Index)
{
    if (Index < 0 || Index > FrameList.count())
    {
        // Index is outside of the bounds of TowerFrame
        return 0;
    }
    else
    {
        frameptr curr = FrameList.at(Index);
        frameptr n = new Frame;
        n->FDuration = curr->FDuration;

        for (int i = 0; i < FHEIGHT; i++)
        {
            for (int j = 0; j < FWIDTH; j++)
            {
                n->WorkArea[i][j] = curr->WorkArea[i][j];
            }
        }

        TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(curr->FDuration));
        FrameList.append(n);
        FrameCount++;
    }
    return 1;
}

void TowerFrame::AddFrame(QTime Duration, int Position)
{
    frameptr n = new Frame;
    n->FDuration = SanitizeTime(Duration);

    for (int i = 0; i < FHEIGHT; i++)
    {
        for (int j = 0; j < FWIDTH; j++)
        {
            n->WorkArea[i][j] = QColor(Qt::black);
        }
    }

    FrameList.insert(Position, n);
    FrameCount++;
    TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(n->FDuration));
}

int TowerFrame::AddFrame(int Index, int Position)
{
    if (Index < 0 || Index > FrameList.count())
    {
        // Index is outside of the bounds of TowerFrame
        return 0;
    }
    else
    {
        frameptr curr = FrameList.at(Index);
        frameptr n = new Frame;
        n->FDuration = curr->FDuration;

        for (int i = 0; i < FHEIGHT; i++)
        {
            for (int j = 0; j < FWIDTH; j++)
            {
                n->WorkArea[i][j] = curr->WorkArea[i][j];
            }
        }

        TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(curr->FDuration));
        FrameList.insert(Position, n);
        FrameCount++;
    }
    return 1;
}

int TowerFrame::DeleteFrame(int Position)
{
    frameptr curr = FrameList.at(Position);
    if (FrameList.count() < Position || Position < 0)
    {
        // Index is out of FrameList bounds
        return 0;
    }
    else
    {
        // Subtract qtime fduration from qtime tduration
        TDuration = TDuration.addMSecs(curr->FDuration.msecsTo(QTime(0,0,0,0)));
        FrameList.removeAt(Position);
        FrameCount--;
    }
    delete(curr);
    return 1;
}

int TowerFrame::MoveFrame(int IndexFrom, int IndexTo)
{
    if (IndexFrom < 0 || IndexTo < 0 ||
        IndexFrom > FrameList.count() || IndexTo > FrameList.count())
    {
        // IndexFrom or IndexTo are outside of the bounds of FrameList
        return 0;
    }
    else
    {
        FrameList.move(IndexFrom,IndexTo);
        return 1;
    }
}

int TowerFrame::ColorCell(int Index, int row, int column, QColor Color)
{
    if (Index < 0 || Index > FrameList.count() || row < 0 || column < 0 ||
        row > FHEIGHT || column > FWIDTH)
    {
        // Index, row, or column are out of bounds
        return 0;
    }
    else
    {
        if (!Color.isValid())
        {
            Color = QColor(Qt::black);
        }
        FrameList.at(Index)->WorkArea[row][column] = Color;
    }
    return 1;
}

int TowerFrame::ColorCell(int row, int column, QColor Color)
{
    if (row < 0 || column < 0 || row > FHEIGHT || column > FWIDTH)
    {
        // Row, or column are out of bounds
        return 0;
    }
    else
    {
        if (!Color.isValid())
        {
            Color = QColor(Qt::black);
        }

        if(currFrame != NULL)
           currFrame->WorkArea[row][column] = Color;
        else
            return 0;

    }
    return 1;
}

int TowerFrame::GetDuration()
{
    return (QTime(0,0,0,0).msecsTo(TDuration));
}

int TowerFrame::SetFrameDuration(QTime Duration, int Index)
{
    if (Index < 0 || Index > FrameList.count())
    {
        // Index is out of FrameList bounds
        return 0;
    }
    else
    {
        FrameList.at(Index)->FDuration = SanitizeTime(Duration);
    }
    return 1;
}

/*
 *
 * Functions below exist only for testing at this point, in the future may be moved to
 * production or to another file completely once GUI testing is possible
 *
 */

// NOTE: If this function is removed this file should no longer include iostream
void TowerFrame::PrintTower()
{
    std::cout << std::endl;
    for (int i = 0; i < FrameList.count(); i++)
    {
        std::cout << std::endl;
        std::cout << "Frame: " << i << " size: " << sizeof(FrameList[i]->WorkArea) << std::endl;
        std::cout << FrameList[i] << std::endl;
        std::cout << FrameList[i]->FDuration.msec() << std::endl;
        for (int j = 0; j < FHEIGHT; j++)
        {
            for (int k = 0; k < FWIDTH; k++)
            {
                std::cout << FrameList[i]->WorkArea[j][k].red() << " " <<
                             FrameList[i]->WorkArea[j][k].green() << " " <<
                             FrameList[i]->WorkArea[j][k].blue() << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << TDuration.minute() << ":" << TDuration.second() << "." << TDuration.msec() << std::endl;
}
