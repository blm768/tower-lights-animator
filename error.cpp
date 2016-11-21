#include <QMessageBox>
#include <QString>
#include <string.h>


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
}

void errOpeningTan()
{
  QMessageBox errorBox;
  errorBox.setIcon(QMessageBox::Warning);
  errorBox.setText("Error opening tan file");
  errorBox.setStandardButtons(QMessageBox::Ok);
  errorBox.exec();
}

void errBadTan()
{
  QMessageBox errorBox;
  errorBox.setIcon(QMessageBox::Warning);
  errorBox.setText(".tan file empty or misformed");
  errorBox.setStandardButtons(QMessageBox::Ok);
  errorBox.exec();
}

void errBadHeaderStamp(int linenum)
{
  QMessageBox errorBox;
  errorBox.setIcon(QMessageBox::Warning);
  errorBox.setText(QString("Error at line %1: The height or width of the tan file is incorrect").arg(linenum));
  errorBox.setStandardButtons(QMessageBox::Ok);
  errorBox.exec();
}

void errBadTimeStamp(int linenum)
{
  QMessageBox errorBox;
  errorBox.setIcon(QMessageBox::Warning);
  errorBox.setText(QString("Error in timestamp at line %1").arg(linenum));
  errorBox.setStandardButtons(QMessageBox::Ok);
  errorBox.exec();
}
