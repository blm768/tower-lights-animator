#include <QTime>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "tanIO.h"

using namespace std;

int lineNum = 0;
int frameCount = 0;

void LoadTan(string fileName, TowerFrame * animation)
{
    QTime previousTime = QTime(0,0,0,0);
    QTime newTime = QTime(0,0,0,0);
    ifstream tanFile;
    string line;
    int inputType;
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
    if((inputType = GetMetaData(line)) == 1){
        frameWidth = 4;
        frameHeight = 10;
    }
    else if(inputType == 2){
        frameWidth = 12;
        frameHeight = 20;
    }
    else
        return;

    int frameLine = 0;

    getline(tanFile, line);        // skip intial 00:00.000 line? or can it be other than that?
                                   // if so we need to make sure that previous time is set right

    animation->CreateNewFrame();

    while(getline(tanFile, line))
    {
        if(frameLine == 9){
            ProcessValues(animation, line);
            getline(tanFile, line);
            newTime = GetNewTime(line);
            animation->AddColoredFrame(frameCount, previousTime, newTime);
            previousTime = newTime;
            animation->CreateNewFrame();
        }
        else
            ProcessValues(animation, line);

    }
}

int GetMetaData(string line)
{
    const char * tok;
    int height;
    int width;


    tok = (const char *) strtok((char *) line.c_str(), " ");
    if(!(frameCount = atoi(tok)))
        Error(tok);
    tok = strtok(NULL, " ");
    if(!(height = atoi(tok)))
        Error(tok);
    tok = strtok(NULL, " ");
    if(!(width = atoi(tok)))
        Error(tok);

    if((tok = strtok(NULL, " ")))
        Error(tok);

    if(width == 4 && height == 10)
        return 1;
    else if (width == 12 && height == 20)
        return 2;
    else{
        cout << "Error at line " << lineNum << ". The height or width of the tan file is incorrect\n";
        return -1;
    }
}

QTime GetNewTime(string line)
{
    const char * tok;
    int mins = 0;
    int secs = 0;
    int ms   = 0;

    tok = (const char *) strtok((char *) line.c_str(), ":");
    mins = atoi(tok);
    tok = strtok(NULL, ".");
    secs = atoi(tok);
    tok = strtok(NULL , " ");
    ms = atoi(tok);

    return QTime(0,mins,secs,ms);

}

void ProcessValues(animation, line)
{

}

void ExportAnimationTan(TowerFrame * animation);
void ExportAnimationProject(TowerFrame * animation);

void Error(const char * tok)
{
    cout << "Error in tan file at line " << lineNum << " and token " << tok << "\n";
}
