#ifndef TANIO_H
#define TANIO_H
#include <QTime>
#include <iostream>
#include <cstring>
#include <string>
#include "towerframe.h"

int   LoadTan(std::string fileName, Animation * animation);
int   GetMetaData(std::string line);
int   GetNewTime(std::string line, QTime * nTime);
int   ProcessValues(Animation * animation, std::string line, int width, int level);
void  Error(const char * tok);



#endif // TANIO_H
