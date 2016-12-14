/*
 * Tower Lights Animator
 * mainwindow.h
 *
 * Worked on by:  Paden Rumsey and others
 * Paden Rumsey: Worked on the close event of the mainwindow
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mainmenu.h"
#include "towerframe.h"

class MainWindow;

/*!
 * \brief The main application window
 *
 * Contains and initializes the top-level application widgets
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

signals:
    /*!
     * \brief Triggered when the animation is changed
     */
    void animationSet(Animation* animation);

public slots:
    void setAnimation(Animation* animation);

private:
    MainMenu _mainMenu;
    Animation* _animation;
};

#endif // MAINWINDOW_H
