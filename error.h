/*
 * Tower Lights Animator
 * error.h 
 * 
 * Worked on by: Paden Rumsey
 */

#ifndef ERROR_H
#define ERROR_H
#include <string.h> 

/* Load Errors */

/*!
 * \brief Tan file is bad
 */
void errBadTan();

/*!
 * \brief sends error that the timestamp in the file is bad
 */
void errBadTimeStamp(int linenum);

/*!
 * \brief sends error that a towerl line in the file is bad
 */
void errBadTanLine(const char * tok, int linenum);

/*!
 * \brief sends error that there was a problem opening the file
 */
void errOpeningTan();

/*!
 * \brief sends error that the tower header is bad
 */
void errBadHeaderStamp(int linenum);

/*!
 * \brief sends error that there is no frame to shift yet
 */
void errNoShift();

#endif // ERROR_H
