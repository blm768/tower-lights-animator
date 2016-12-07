#include <QColor>
#include <QTime>
#include <QList>
#include <iostream>
#include "towerframe.h"

extern bool wrap;

Animation::Animation()
{
    TDuration = 0;
    currFrame = NULL;
}

bool Animation::FrameSelected()
{
    return (currFrame != NULL);
}

int Animation::SelectFrame(int Index)
{
    if (!IsValidFrame(Index)){
        return 0;
    } else {
        currFrame = FrameList.at(Index);
    }
    return 1;
}

int Animation::SetSelectedColor(int row, int column, QColor Color)
{
    if (currFrame == NULL)
    {
        // currFrame is not set
        return 0;
    }
    if (!Color.isValid())
    {
        Color = QColor(Qt::black);
    }
    currFrame->WorkArea[row][column] = Color;
    return 1;
}

QColor Animation::GetSelectedColor(int row, int column)
{
    if (currFrame == NULL)
    {
        // currFrame is not set
        return (QColor(Qt::black));
    }
    return (currFrame->WorkArea[row][column]);
}

int Animation::SanitizeTime(int OutTime)
{
    int MinMS = 25;
    if (OutTime < 0)
    {
        OutTime = MinMS;
    }
    else
    {
        int remainder = (OutTime % MinMS);
        if (remainder != 0)
        {
            OutTime += (MinMS - remainder);
        }
    }
    return OutTime;
}

Frame* Animation::GetFrame(int index) {
    if (!IsValidFrame(index))
    {
        return FrameList.at(0);
    }
    return FrameList.at(index);
}

int Animation::InsertFrame(int index, Frame* frame)
{
    if (index < 0 || index > FrameList.length())
    {
        return 0;
    }
    FrameList.insert(index, frame);
    return 1;
}

int Animation::FrameCount() const {
    return FrameList.count();
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
        return (FrameList.at(Index)->FDuration);
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
        currFrame->FDuration = 0;
        int mpTime = QTime(0,0,0,0).msecsTo(pTime);
        int mnTime = QTime(0,0,0,0).msecsTo(nTime);

        if (pTime > nTime)
        {
            //Error handling?
            currFrame->FDuration = 25;
        }
        else
        {
        currFrame->FDuration += (mnTime - mpTime); //currFrame->FDuration.addMSecs(pTime.msecsTo(nTime));
        currFrame->FDuration = SanitizeTime(currFrame->FDuration);
        }

        FrameList.append(currFrame);
        TDuration += (mnTime - mpTime); //TDuration.addMSecs(pTime.msecsTo(nTime));
        return 1;
    }
    else
        return 0;
}

void Animation::AddFrame(int Duration)
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
    TDuration = n->FDuration;
}

int Animation::CopyFrame(int Index)
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

        TDuration += curr->FDuration;
        FrameList.append(n);
    }
    return 1;
}

void Animation::AddFrame(int Duration, int Position)
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
    TDuration += n->FDuration;
}

int Animation::CopyFrame(int Index, int Position)
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

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
    }
    return 1;
}

int Animation::ShiftFrameLU(int Index, int Position)
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

        for (int i = 0; i < (FHEIGHT); i++)
        {
            for (int j = 0; j < (FWIDTH); j++)
            {
                if(wrap == false)
                {
                    if(i == (FHEIGHT - 1) || j == (FWIDTH - 1))
                     n->WorkArea[i][j] = QColor(Qt::black);
                    else
                     n->WorkArea[i][j] = curr->WorkArea[i + 1][j + 1];
                }
                else
                {
                    if(i == (FHEIGHT - 1) || j == (FWIDTH - 1))
                     n->WorkArea[i][j] = QColor(Qt::black);
                    else
                     n->WorkArea[i][j] = curr->WorkArea[i + 1][j + 1];
                }
            }
        }

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
    }
    return 1;
}

int Animation::ShiftFrameL(int Index, int Position)
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

        for (int i = 0; i < (FHEIGHT); i++)
        {
            for (int j = 0; j < (FWIDTH); j++)
            {
                if(wrap == true)
                {
                    if(j == (FWIDTH - 1))
                    { /* do nothing */ }
                    else if(j == 0)
                    {
                        n->WorkArea[i][FWIDTH - 1] = curr->WorkArea[i][j];
                        n->WorkArea[i][j] = curr->WorkArea[i][j + 1];
                    }
                    else
                        n->WorkArea[i][j] = curr->WorkArea[i][j + 1];
                }
                else
                {
                    if(j == (FWIDTH - 1))
                        n->WorkArea[i][j] = QColor(Qt::black);
                    else
                        n->WorkArea[i][j] = curr->WorkArea[i][j + 1];
                }
            }
        }

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
    }
    return 1;
}

int Animation::ShiftFrameLD(int Index, int Position)
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

        for (int i = 0; i < (FHEIGHT); i++)
        {
            for (int j = 0; j < (FWIDTH); j++)
            {
                if(i == 0 || j == (FWIDTH - 1))
                    n->WorkArea[i][j] = QColor(Qt::black);
                else
                    n->WorkArea[i][j] = curr->WorkArea[i - 1][j + 1];
            }
        }

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
    }
    return 1;
}

int Animation::ShiftFrameUp(int Index, int Position)
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

        for (int i = 0; i < (FHEIGHT); i++)
        {
            for (int j = 0; j < (FWIDTH); j++)
            {
                if(wrap == true)
                {
                    if(i == (FHEIGHT - 1))
                    { /* do nothing */ }
                    else if(i == 0)
                    {
                        n->WorkArea[FHEIGHT - 1][j] = curr->WorkArea[i][j];
                        n->WorkArea[i][j] = curr->WorkArea[i + 1][j];
                    }
                    else
                        n->WorkArea[i][j] = curr->WorkArea[i + 1][j];
                }
                else
                {
                    if(i == (FHEIGHT - 1))
                        n->WorkArea[i][j] = QColor(Qt::black);
                    else
                        n->WorkArea[i][j] = curr->WorkArea[i + 1][j];
                }
            }
        }

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
    }
    return 1;
}

int Animation::ShiftFrameDown(int Index, int Position)
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

        for (int i = 0; i < (FHEIGHT); i++)
        {
            for (int j = 0; j < (FWIDTH); j++)
            {
                if(wrap == true)
                {
                    if(i == 0)
                        n->WorkArea[i][j] = curr->WorkArea[FHEIGHT - 1][j];
                    else
                        n->WorkArea[i][j] = curr->WorkArea[i - 1][j];
                 }
                else
                {
                    if(i == 0)
                        n->WorkArea[i][j] = QColor(Qt::black);
                    else
                        n->WorkArea[i][j] = curr->WorkArea[i - 1][j];
                }
            }
        }

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
    }
    return 1;
}

int Animation::ShiftFrameRU(int Index, int Position)
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

        for (int i = 0; i < (FHEIGHT); i++)
        {
            for (int j = 0; j < (FWIDTH); j++)
            {
                if(i == (FHEIGHT - 1) || j == 0)
                    n->WorkArea[i][j] = QColor(Qt::black);
                else
                    n->WorkArea[i][j] = curr->WorkArea[i + 1][j - 1];
            }
        }

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
    }
    return 1;
}

int Animation::ShiftFrameR(int Index, int Position)
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

        for (int i = 0; i < (FHEIGHT); i++)
        {
            for (int j = 0; j < (FWIDTH); j++)
            {
                if(wrap == true)
                {
                    if(j== 0)
                        n->WorkArea[i][j] = curr->WorkArea[i][FWIDTH - 1];
                    else
                        n->WorkArea[i][j] = curr->WorkArea[i][j - 1];
                }
                else
                {
                    if(j == 0)
                        n->WorkArea[i][j] = QColor(Qt::black);
                    else
                        n->WorkArea[i][j] = curr->WorkArea[i][j - 1];
                }
            }
        }

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
    }
    return 1;
}

int Animation::ShiftFrameRD(int Index, int Position)
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

        for (int i = 0; i < (FHEIGHT); i++)
        {
            for (int j = 0; j < (FWIDTH); j++)
            {
                if(i == 0 || j == 0)
                    n->WorkArea[i][j] = QColor(Qt::black);
                else
                    n->WorkArea[i][j] = curr->WorkArea[i - 1][j - 1];
            }
        }

        TDuration += curr->FDuration;
        FrameList.insert(Position, n);
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
        TDuration -= curr->FDuration;
        FrameList.removeAt(Position);
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

int Animation::SetCellColor(int Index, int row, int column, QColor Color)
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
    return TDuration;
}

int Animation::SetFrameDuration(int Duration, int Index)
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
 * Testing
 *
 *
 *
void Animation::PrintTower()
{
    QString qs;
    QTime temp;
    std::cout << std::endl;
    for (int i = 0; i < FrameList.count(); i++)
    {
        std::cout << std::endl;
        std::cout << "Frame: " << i << " size: " << sizeof(FrameList[i]->WorkArea) << std::endl;
        std::cout << FrameList[i] << std::endl;
        qs = FrameList[i]->toQTime().toString("mm:ss.zzz");
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
    //qs = TDuration.toString("mm:ss.zzz");
    std::cout << qs.toStdString() << std::endl;
    for (int i = 0; i < FrameList.count(); i++)
    {
        std::cout << std::endl << FrameList.at(i)->FDuration << std::endl;
    }
}
*/
