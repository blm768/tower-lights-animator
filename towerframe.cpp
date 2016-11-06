#include <QColor>
#include <QTime>
#include <QList>
#include <iostream>
#include "towerframe.h"


TowerFrame::TowerFrame()
{
    TDuration.setHMS(0, 0, 0, 0);
}

QTime TowerFrame::SanitizeTime(QTime InTime)
{
    QTime OutTime = InTime;
    int MinMS = 25;
    if (!OutTime.isValid())
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

void TowerFrame::AddColoredFrame(int Position, QTime pTime, QTime nTime)
{
    currFrame->FDuration = SanitizeTime(currFrame->FDuration.addMSecs(pTime.msecsTo(nTime)));
    FrameList.append(currFrame);
    TDuration = TDuration.addMSecs(pTime.msecsTo(nTime));
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
    TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(Duration));
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
    TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(Duration));
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
        FrameList.at(Index)->FDuration = Duration;
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
