#include <QMessageBox>
#include <QString>
#include <string.h>

extern int lineNum;            //used for handling headers... eventually
extern int frameCount;         //check to see if we collected all the frames

void errBadTanLine(const char * tok, int linenum)
{
    QMessageBox errorBox;
    QString token = tok;
    errorBox.setIcon(QMessageBox::Warning);
    errorBox.setText(QString("Error in tan file at line %1 and token %2")
                     .arg(linenum)
                     .arg(token));
    errorBox.setStandardButtons(QMessageBox::Ok);
    errorBox.exec();
    lineNum = 0;
    frameCount = 1;
}

void errOpeningTan()
{
  QMessageBox errorBox;
  errorBox.setIcon(QMessageBox::Warning);
  errorBox.setText("Error opening tan file");
  errorBox.setStandardButtons(QMessageBox::Ok);
  errorBox.exec();
  lineNum = 0;
  frameCount = 1;
}

void errBadTan()
{
  QMessageBox errorBox;
  errorBox.setIcon(QMessageBox::Warning);
  errorBox.setText("Error .tan file empty or misformed");
  errorBox.setStandardButtons(QMessageBox::Ok);
  errorBox.exec();
  lineNum = 0;void errNoShift();
  frameCount = 1;
}

void errBadHeaderStamp(int linenum)
{
  QMessageBox errorBox;
  errorBox.setIcon(QMessageBox::Warning);
  errorBox.setText(QString("Error at line %1: The height or width of the tan file is incorrect").arg(linenum));
  errorBox.setStandardButtons(QMessageBox::Ok);
  errorBox.exec();
  lineNum = 0;
  frameCount = 1;
}

void errBadTimeStamp(int linenum)
{
  QMessageBox errorBox;
  errorBox.setIcon(QMessageBox::Warning);
  errorBox.setText(QString("Error in timestamp at line %1").arg(linenum));
  errorBox.setStandardButtons(QMessageBox::Ok);
  errorBox.exec();
  lineNum = 0;
  frameCount = 1;
}

void errNoShift()
{
    QMessageBox errorBox;
    errorBox.setIcon(QMessageBox::Warning);
    errorBox.setText(QString("Error: There is no frame to shift"));
    errorBox.setStandardButtons(QMessageBox::Ok);
    errorBox.exec();
}
