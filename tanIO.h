/*
 * Tower Lights Animator
 * tanIO.h
 * 
 * Worked on by: Paden Rumsey, Max Flinn
 * Paden's Contributions: All functions except SaveProject/SaveTan
 * Max's Contributions: SaveProject/SaveTan
 */

#ifndef TANIO_H
#define TANIO_H
#include <QTime>
#include <iostream>
#include <cstring>
#include <string>
#include "towerframe.h"

      //! Loads the tan/project file if there is one to load

int   LoadTan(std::string fileName, Animation * animation);

      //! Reads the header stamp to determin if proj/tan file

int   GetMetaData(std::string line);

      //! Reads the new time and calculates duration from previous timestamp

int   GetNewTime(std::string line, QTime * nTime);

      //! Processes the lines of the RGB  values

int   ProcessValues(Animation * animation, std::string line, int width, int level);

int   SaveTan( std::string fileName, Animation * tower );
int   SaveProject( std::string fileName, Animation * tower );

#endif // TANIO_H
