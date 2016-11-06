#ifndef TANIO_H
#define TANIO_H
#include <iostream>
#include <cstring>
#include <string>
#include "towerframe.h"

void loadTan(std::string fileName, TowerFrame * animation);
int  handleHeader(std::string line);
void exportAnimationTan(TowerFrame * animation);
void exportAnimationProject(TowerFrame * animation);
void error(const char * tok);



#endif // TANIO_H
