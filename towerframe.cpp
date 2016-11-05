#include <QColor>
#include <QTime>
#include <QList>
#include <iostream>
#include "towerframe.h"


TowerFrame::TowerFrame()
{
    TDuration.setHMS(0, 0, 0, 0);
}

int TowerFrame::AddFrame(QTime Duration)
{
    frameptr n = new Frame;
    if (Duration >= QTime(0,0,0,050))
    {
        n->FDuration = Duration;
    } else {
        n->FDuration = QTime(0,0,0,050);
    }

    for (int i = 0; i < FWIDTH; i++)
    {
        for (int j = 0; j < FHEIGHT; j++)
        {
            n->WorkArea[i][j] = QColor(Qt::black);
        }
    }

    FrameList.append(n);
    TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(Duration));
    return 1;
}

int TowerFrame::AddFrame(int Index)
{
    frameptr curr = FrameList.at(Index);
    frameptr n = new Frame;
    n->FDuration = curr->FDuration;

    for (int i = 0; i < FWIDTH; i++)
    {
        for (int j = 0; j < FHEIGHT; j++)
        {
            n->WorkArea[i][j] = curr->WorkArea[i][j];
        }
    }

    TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(curr->FDuration));
    FrameList.append(n);
    return 1;
}

int TowerFrame::AddFrame(QTime Duration, int Position)
{
    frameptr n = new Frame;
    if (Duration >= QTime(0,0,0,050))
    {
        n->FDuration = Duration;
    } else {
        n->FDuration = QTime(0,0,0,050);
    }

    for (int i = 0; i < FWIDTH; i++)
    {
        for (int j = 0; j < FHEIGHT; j++)
        {
            n->WorkArea[i][j] = QColor(Qt::black);
        }
    }

    FrameList.insert(Position, n);
    TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(Duration));
    return 1;
}

int TowerFrame::AddFrame(int Index, int Position)
{
    frameptr curr = FrameList.at(Index);
    std::cout << "Hi?" << std::endl;
    frameptr n = new Frame;
    n->FDuration = curr->FDuration;

    for (int i = 0; i < FWIDTH; i++)
    {
        for (int j = 0; j < FHEIGHT; j++)
        {
            n->WorkArea[i][j] = curr->WorkArea[i][j];
        }
    }

    TDuration = TDuration.addMSecs(QTime(0,0,0,0).msecsTo(curr->FDuration));
    FrameList.insert(Position, n);
    return 1;
}

void TowerFrame::DeleteFrame(int Index)
{
    frameptr curr = FrameList.at(Index);
    std::cout << std::endl << "Hi?" << (QTime(0,0,0,0).msecsTo(curr->FDuration)) << std::endl;
    FrameList.removeAt(Index);
    delete(curr);
}

int TowerFrame::MoveFrame(int IndexFrom, int IndexTo)
{
    FrameList.move(IndexFrom,IndexTo);
    return 1;
}

void TowerFrame::ColorCell(int Index, int row, int column, QColor Color)
{
    FrameList.at(Index)->WorkArea[row][column] = Color;
}

int TowerFrame::GetDuration()
{
    return (QTime(0,0,0,0).msecsTo(TDuration));
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
        for (int j = 0; j < FWIDTH; j++)
        {
            for (int k = 0; k < FHEIGHT; k++)
            {
                std::cout << FrameList[i]->WorkArea[j][k].red() << " " <<
                             FrameList[i]->WorkArea[j][k].green() << " " <<
                             FrameList[i]->WorkArea[j][k].blue() << " ";
            }
            std::cout << std::endl;
        }
    }
}
