#ifndef MAINMENU_H
#define MAINMENU_H

#include <QObject>
#include <QAction>
#include <QMenu>

#include "frame.h"

// Forward declaration
class MainWindow;

// Holds the main menus and actions
class MainMenu : public QObject
{
    Q_OBJECT
public:
    explicit MainMenu(MainWindow* window);

signals:
    // Called when an animation is created or loaded.
    void animationSet(Animation* animation);
    // Called on menu events
    void cut();
    void copy();
    void paste();
    void close();

private slots:
    // Slots corresponding to menu actions
    void newFile();
    void saveFile();
    void saveFileAs();
    void exportFile();

private:
    // Menu actions
    QAction* actNewFile;
    QAction* actSaveFile;
    QAction* actSaveAs;
    QAction* actExport;
    QAction* actExit;
    QAction* Cut;
    QAction* Copy;
    QAction* Paste;

    // Menus
    QMenu* menuFile;
    QMenu* menuEdit;
};

#endif // MAINMENU_H
