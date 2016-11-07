#include <QTime>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "tanIO.h"

using namespace std;

int lineNum = 0;            //used for handling headers... eventually
int frameCount = 1;         //check to see if we collected all the frames

int LoadTan(string fileName, TowerFrame * animation)
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
        return 0;
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
        return 0;

    int frameLine = 1;

    getline(tanFile, line);        // skip intial 00:00.000 line? or can it be some other time than that?
    lineNum++;                     // if so we need to make sure that previous time is set right


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
        lineNum++;
        if(frameLine == frameHeight){
            if(!(ProcessValues(animation, line, frameWidth, frameLine)))
                return 0;
            if(!getline(tanFile, line)){
                //frameCount++;
                //lineNum++;
                if(!animation->AddColoredFrame(QTime(0,0,0,0), QTime(0,0,0,25)))
                    return 0;
                break;
            }
            if(!GetNewTime(line, &newTime))
                return 0;
            if(!animation->AddColoredFrame(previousTime, newTime))
                return 0;
            previousTime = newTime;
            frameCount++;
            animation->CreateNewFrame();
            frameLine = 1;
        }
        else{
            if(!(ProcessValues(animation, line, frameWidth, frameLine)))
                return 0;
            frameLine++;
        }
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
        return 0;
    }
}

int GetNewTime(string line, QTime * nTime)
{
    const char * tok;
    int mins = 0;
    int secs = 0;
    int ms   = 0;

    if(!(tok = strtok((char *) line.c_str(), ":"))){              //get minutes
        cout << "Error in timestamp at line " << lineNum;
        return 0;
    }
    else
        mins = atoi(tok);
    if(!(tok = strtok(NULL, "."))){                                //get seconds
        cout << "Error in timestamp at line " << lineNum;
        return 0;
    }
    else
        secs = atoi(tok);
    if(!(tok = strtok(NULL , " "))){                               //get milliseconds
        cout << "Error in timestamp at line " << lineNum;
        return 0;
    }
    else
        ms = atoi(tok);

    nTime->setHMS(0,mins, secs, ms);

    return 1;

}

int ProcessValues(TowerFrame * animation, string line, int width, int level)
{
    const char * tok;
    QColor newColor;
    int red;
    int green;
    int blue;

    //must initally break the first grouping then loop

    if(!(tok = strtok((char *) line.c_str(), " ")))
    {
        Error(tok);
        return 0;
    }
    else
        red = atoi(tok);
    if(!(tok = strtok(NULL, " ")))
    {
        Error(tok);
        return 0;
    }
    else
        green = atoi(tok);
    if(!(tok = strtok(NULL, " ")))
    {
        Error(tok);
        return 0;
    }
    else
        blue = atoi(tok);

    //animation->ColorCell(1, level, QColor(red, green, blue));

    cout << "red: " << red << " ";
    cout << "blue: " << blue << " ";
    cout << "green:" << green << "\n";

    //start looping through the rest

    for (int i = 1; i < width; i++)
    {

        if(!(tok = strtok((char *) line.c_str(), " ")))
        {
            Error(tok);
            return 0;
        }
        else
            red = atoi(tok);
        if(!(tok = strtok(NULL, " ")))
        {
            Error(tok);
            return 0;
        }
        else
            green = atoi(tok);
        if(!(tok = strtok(NULL, " ")))
        {
            Error(tok);
            return 0;
        }
        else
            blue = atoi(tok);

        cout << "red: " << red << " ";
        cout << "blue: " << blue << " ";
        cout << "green:" << green << "\n";

        //animation->ColorCell(i, level, QColor(red, green, blue));
    }
}

void ExportAnimationTan(TowerFrame * animation);
void ExportAnimationProject(TowerFrame * animation);

void Error(const char * tok)
{
    cout << "Error in tan file at line " << lineNum << " and token " << tok << "\n";
}
