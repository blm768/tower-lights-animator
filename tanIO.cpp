#include <QTime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <string>
#include "tanIO.h"

using namespace std;

int lineNum = 0;            //used for handling headers... eventually
int frameCount = 1;         //check to see if we collected all the frames

int LoadTan(string fileName, Animation * animation)
{
    QTime previousTime = QTime(0,0,0,0);
    QTime newTime = QTime(0,0,0,0);
    ifstream tanFile;
    string line;
    int inputType;
    int frameWidth = 0;
    int frameHeight = 0;
    bool foundFirst = false;

    tanFile.open(fileName, ifstream::in);

    if(!tanFile){
        cout << "Error opening .tan file\n" << fileName;
        return 0;
    }

    /* iterate to the first time stamp
     * if we find a colon we set foundFirst
     * to true
     */

    while(getline(tanFile, line))
    {
        lineNum++;
        if(line.find(":") != std::string::npos)
        {
            foundFirst = true;
            break;
        }
    }

    /* if we found a frame we
     * rewind the ifstream to the beginning
     * and then we set lineNum to the one
     * before that and iterate to the
     * stamp that tells us if this
     * is a project or .tan file
     */

    if(foundFirst == true)
    {
        lineNum = lineNum - 1;
        tanFile.seekg(0, ios::beg);
        for(int i = 0; i < lineNum; i++)
            getline(tanFile, line);
    }
    else
    {
        cout << "ERROR tan file is misformed or has no frames";
        return 0;
    }

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
                frameCount++;
                lineNum++;
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

    animation->PrintTower();

    return 1;
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

int ProcessValues(Animation * animation, string line, int width, int level)
{
    const char * tok;
    QColor newColor;
    int red;
    int green;
    int blue;
    int widthOffset = 0;
    int heightOffset = 0;

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

    if(width == 4)
    {
        widthOffset = 4;
        heightOffset = 5;
    }

    animation->ColorCell((level-1) + heightOffset, 0 + widthOffset, QColor(red, green, blue));

    //start looping through the rest

    for (int i = 1; i < width; i++)
    {

        if(!(tok = strtok(NULL, " ")))
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

        animation->ColorCell((level-1) + heightOffset, i + widthOffset, QColor(red, green, blue));
    }
    return 1;
}

//Save functions
//Saves the animation in a tan file which is
//readable by the Tower of Lights player
int SaveTan( string fileName, Animation *tower ) {

    int index = 0;
    QColor current;
    int r,g,b;
    int timeMs = 0;
    int timeS = 0;
    int timeM = 0;

    ofstream output;
    output.open( fileName );

    output << "0.3" << endl;
    output << "NoAudioFile" << endl;
    output << /*Insert last color here*/endl;

    while((tower->GetFrameDuration(index))) {

        while( timeMs > 999 ) {

            timeMs -= 1000;
            timeS++;
        }
        while( timeS > 59 ) {

            timeS -= 60;
            timeM++;
        }
        output << setfill('0') << setw(2) << timeM << ":";
        output << setfill('0') << setw(2) << timeS << ".";
        output << setfill('0') << setw(3) << timeMs << endl;

        //Sizes used are the static 14x20 that the tower is statically allocated to
        for( int i=5; i<15; i++ ) {
            for( int j=5; j<10; j++ ) {

                //Get color of a cell and write it to the file
                current = tower->GetCellColor(index, i, j);
                current.getRgb(&r, &g, &b);
                output << r << " " << g << " " << b << " ";
            }
            cout << endl;
            output << endl;
        }
        timeMs += tower->GetFrameDuration(index);
        index++;
    }
    output.close();
    return 0;
}

    //Saves animation to a project file, time is in ms
    //and tower width is 14x20
int SaveProject( string fileName, Animation *tower ) {

    int index = 0;
    QColor current;
    int r,g,b;
    int time = 0;

    ofstream output;
    output.open( fileName );

    //Following writes put the time into the correct format
    output << "0.4" << endl;
    output << "NoAudioFile" << endl;
    output << /*Insert last color here*/endl;

    while((tower->GetFrameDuration(index))) {

        output << time << endl;

        for( int i=0; i<20; i++ ) {
            for( int j = 0; j<14; j++ ) {

                //Get color of a cell and write its RGB values
                current = tower->GetCellColor(index, i, j);
                current.getRgb(&r, &g, &b);
                output << r << " " << g << " " << b << " ";
            }
            cout << endl;
            output << endl;
        }
        time += tower->GetFrameDuration(index);
        index++;
    }
    output.close();
    return 0;
}

void Error(const char * tok)
{
    cout << "Error in tan file at line " << lineNum << " and token " << tok << "\n";
}
