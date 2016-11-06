#include <QTime>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "tanIO.h"

using namespace std;

int lineNum = 0;            //used for handling headers... eventually
int frameCount = 1;         //check to see if we collected all the frames

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

    if((inputType = GetMetaData(line)) == 1){      //if this format is a .tan file
        frameWidth = 4;
        frameHeight = 10;
    }
    else if(inputType == 2){                       //if this format is a project file
        frameWidth = 12;
        frameHeight = 20;
    }
    else
        return;

    int frameLine = 1;

    getline(tanFile, line);        // skip intial 00:00.000 line? or can it be some other time than that?
                                   // if so we need to make sure that previous time is set right


    /* The order goes. If we are at the end of a frame we
     * want to process the last line of the frame and then
     * get the next frame timestamp (if we aren't at the end
     * of the file else break). Then process the next timestamp
     * and use that and the previous timestamp to calculate the
     * duration. The new time becomes the previous time. And then
     * we create a new frame to be processed. If not at last line
     * of frame just process the line
     */

    animation->CreateNewFrame();

    while(getline(tanFile, line))
    {
        if(frameLine == frameHeight){
            ProcessValues(animation, line, frameWidth, frameLine);
            if(!getline(tanFile, line)){
                frameCount++;
                animation->AddColoredFrame(QTime(0,0,0,0), QTime(0,0,0,25));
                break;
            }
            newTime = GetNewTime(line);
            animation->AddColoredFrame(previousTime, newTime);
            previousTime = newTime;
            frameCount++;
            animation->CreateNewFrame();
        }
        else
            ProcessValues(animation, line, frameWidth, frameLine);

    }
}

int GetMetaData(string line)
{
    const char * tok;
    int height;
    int width;


    tok = (const char *) strtok((char *) line.c_str(), " ");     //framecount
    if(!(frameCount = atoi(tok)))
        Error(tok);
    tok = strtok(NULL, " ");
    if(!(height = atoi(tok)))                                    //height of tower
        Error(tok);
    tok = strtok(NULL, " ");
    if(!(width = atoi(tok)))                                     //width of tower
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

    tok = strtok((char *) line.c_str(), ":");      //get minutes
    mins = atoi(tok);
    tok = strtok(NULL, ".");                                      //get seconds
    secs = atoi(tok);
    tok = strtok(NULL , " ");                                     //get milliseconds
    ms = atoi(tok);

    return QTime(0,mins,secs,ms);

}

void ProcessValues(TowerFrame * animation, string line, int width, int level)
{
    const char * tok;
    QColor newColor;
    int red;
    int green;
    int blue;

    //must initally break the first grouping then loop

    tok = strtok((char *) line.c_str(), " ");
    red = atoi(tok);
    tok = strtok(NULL, " ");
    green = atoi(tok);
    tok = strtok(NULL, " ");
    blue = atoi(tok);

    animation->ColorCell
    //start looping through the rest

    for (int i = 1; i < width; i++)
    {
        tok = strtok(NULL, " ");
        red = atoi(tok);
        tok = strtok(NULL, " ");
        green = atoi(tok);
        tok = strtok(NULL, " ");
        blue = atoi(tok);
    }
}

void ExportAnimationTan(TowerFrame * animation);
void ExportAnimationProject(TowerFrame * animation);

void Error(const char * tok)
{
    cout << "Error in tan file at line " << lineNum << " and token " << tok << "\n";
}
