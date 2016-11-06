#include <QTime>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "tanIO.h"

using namespace std;

int lineNum = 0;
int frameCount = 0;

void loadTan(string fileName, TowerFrame * animation)
{
    QTime previousTime = QTime(0,0,0,0);
    QTime newTime = QTime(0,0,0,0);
    ifstream tanFile;
    string line;
    int inputType;
    int frameLine = 0;
    int frameWidth = 0;
    int frameHeight = 0;

    tanFile.open(fileName, ifstream::in);


    if(!tanFile){
        cout << "Error opening .tan file\n" << fileName;
        return;
    }   

    lineNum = 3;              //temporary placeholder till I hander headers
    getline(tanFile,  line); // we should figure out a better way to handle the header
    getline(tanFile,  line); // but for now I want to get to the meat of the problem
    getline(tanFile,  line); //this line will matter because we are going to see
                                       // whether or not we have a project or tan file
    if((inputType = handleHeader(line)) == 1){
        frameWidth = 4;
        frameHeight = 10;
    }
    else if(inputType == 2){
        frameWidth = 12;
        frameHeight = 20;
    }
    else
        return;

    while(getline(tanFile, line))
    {
        if(frameLine == 0){
            animation->currFrame = new Frame;
            processTime(line);
        }
        else{
            if(frameLine == 9)
                frameLine = 0;

        }


    }



}

int handleHeader(string line)
{
    const char * tok;
    int height;
    int width;


    tok = (const char *) strtok((char *) line.c_str(), " ");
    if(!(frameCount = atoi(tok)))
        error(tok);
    tok = strtok(NULL, " ");
    if(!(height = atoi(tok)))
        error(tok);
    tok = strtok(NULL, " ");
    if(!(width = atoi(tok)))
        error(tok);

    if((tok = strtok(NULL, " ")))
        error(tok);

    if(width == 4 && height == 10)
        return 1;
    else if (width == 12 && height == 20)
        return 2;
    else{
        cout << "Error at line " << lineNum << ". The height or width of the tan file is incorrect\n";
        return -1;
    }
}

void exportAnimationTan(TowerFrame * animation);
void exportAnimationProject(TowerFrame * animation);

void error(const char * tok)
{
    cout << "Error in tan file at line " << lineNum << " and token " << tok << "\n";
}
