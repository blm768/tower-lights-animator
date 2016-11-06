#ifndef TANIO_H
#define TANIO_H
#include <QTime>
#include <iostream>
#include <cstring>
#include <string>
#include "towerframe.h"

void  LoadTan(std::string fileName, TowerFrame * animation);
int   GetMetaData(std::string line);
QTime GetNewTime(std::string line);
void  ProcessValues(TowerFrame * animation, std::string line, int width, int level);
void  ExportAnimationTan(TowerFrame * animation);
void  ExportAnimationProject(TowerFrame * animation);
void  Error(const char * tok);



#endif // TANIO_H
