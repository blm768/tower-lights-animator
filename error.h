#ifndef ERROR_H
#define ERROR_H
#include <string.h> 

/* Load Errors */

//Tan file is bad
void errBadTan();

//Bad timestamp in tan file
void errBadTimeStamp(int linenum);

//Bad line in tan file
void errBadTanLine(const char * tok, int linenum);

//Can't open file
void errOpeningTan();

//Bad width or height or header timestamp
void errBadHeaderStamp(int linenum);

#endif // ERROR_H
