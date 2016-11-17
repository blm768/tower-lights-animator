#include <QColor>
#include <QTime>
#include <QList>
#include <iostream>
#include "towerframe.h"

/*QColor& Frame::cell(size_t x, size_t y) {
    return WorkArea[y][x];
}*/

Animation::Animation()
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
QTime Animation::SanitizeTime(QTime InTime)
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

int Animation::GetFrameDuration(int Index)
{
    if (!IsValidFrame(Index))
    {
        // Index out of bounds
        return 0;
    }
    else
    {
        return (QTime(0,0,0,0).msecsTo(FrameList.at(Index)->FDuration));
    }
}

void Animation::CreateNewFrame()
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

int Animation::AddColoredFrame(QTime pTime, QTime nTime)
{
    if(currFrame != NULL)
    {
        currFrame->FDuration = QTime(0,0,0,0);

        if (pTime > nTime)
        {
            //Error handling?
            currFrame->FDuration = QTime(0,0,0,25);
        }
        else
        {
        currFrame->FDuration = currFrame->FDuration.addMSecs(pTime.msecsTo(nTime));
        currFrame->FDuration = SanitizeTime(currFrame->FDuration);
        }

        FrameList.append(currFrame);
        FrameCount++;
        TDuration = TDuration.addMSecs(pTime.msecsTo(nTime));
        return 1;
    }
    else
        return 0;
}

void Animation::AddFrame(QTime Duration)
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

int Animation::AddFrame(int Index)
{
    if (!IsValidFrame(Index))
    {
        // Index is outside of the bounds of Animation
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

void Animation::AddFrame(QTime Duration, int Position)
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

int Animation::AddFrame(int Index, int Position)
{
    if (!IsValidFrame(Index))
    {
        // Index is outside of the bounds of Animation
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

int Animation::DeleteFrame(int Position)
{
    frameptr curr = FrameList.at(Position);
    if (!IsValidFrame(Position))
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

int Animation::MoveFrame(int IndexFrom, int IndexTo)
{
    if (!IsValidFrame(IndexFrom) || !IsValidFrame(IndexTo))
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

int Animation::ColorCell(int Index, int row, int column, QColor Color)
{
    if (!IsValidFrameCell(Index, row, column))
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

QColor Animation::GetCellColor(int Index, int row, int column)
{
    if (!IsValidFrameCell(Index, row, column))
    {
        // Invalid index/row/column, returns black
        return (QColor(Qt::black));
    }
    else
    {
        return (FrameList.at(Index)->WorkArea[row][column]);
    }
}

int Animation::ColorCell(int row, int column, QColor Color)
{
    if (!IsValidCell(row, column))
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

bool Animation::IsValidCell(int row, int column)
{
    if (row >= 0 && column >= 0 && row < FHEIGHT && column < FWIDTH)
    {
        return true;
    }
    return false;
}

bool Animation::IsValidFrame(int Index)
{
    if (Index >= 0 && Index < FrameList.count())
    {
        return true;
    }
    return false;
}

bool Animation::IsValidFrameCell(int Index, int row, int column)
{
    if (IsValidFrame(Index) && IsValidCell(row, column))
    {
        return true;
    }
    return false;
}

int Animation::GetDuration()
{
    return (QTime(0,0,0,0).msecsTo(TDuration));
}

int Animation::SetFrameDuration(QTime Duration, int Index)
{
    if(!IsValidFrame(Index))
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
void Animation::PrintTower()
{
    QString qs;
    std::cout << std::endl;
    for (int i = 0; i < FrameList.count(); i++)
    {
        std::cout << std::endl;
        std::cout << "Frame: " << i << " size: " << sizeof(FrameList[i]->WorkArea) << std::endl;
        std::cout << FrameList[i] << std::endl;
        qs = FrameList[i]->FDuration.toString("mm:ss.zzz");
        std::cout << qs.toStdString() << std::endl;
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
    qs = TDuration.toString("mm:ss.zzz");
    std::cout << qs.toStdString() << std::endl;
    for (int i = 0; i < FrameList.count(); i++)
    {
        std::cout << std::endl << FrameList.at(i)->toMsec() << std::endl;
    }
}
