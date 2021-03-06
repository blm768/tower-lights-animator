/* CONTRIBUTORS
 * Ben Merritt: outlined the initial signals, slots, and actions
 * Paden Rumsey: Implemented file operations (except export)
 * Max Gilmore: Created Insert and Animation menus, no animations implemented yet
 */
#ifndef MAINMENU_H
#define MAINMENU_H

#include <QObject>
#include <QAction>
#include <QMenu>

#include "towerframe.h"

// Forward declaration
class MainWindow;

/*!
 * \brief Holds the main menus and actions
 */
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
    void rainEffect();

private slots:
    // Slots corresponding to menu actions
    void newFile();
    void saveFile();
    void saveFileAs();
    void openFile();
    void exportFile();

private:
    // Menu actions
    QAction* actNewFile;
    QAction* actSaveFile;
    QAction* actSaveAs;
    QAction* actOpenFile;
    QAction* actExport;
    QAction* actExit;
    QAction* Cut;
    QAction* Copy;
    QAction* Paste;
    QAction* RainEffect;

    // Menus
    QMenu* menuFile;
    QMenu* menuEdit;
    QMenu* menuInsert;
    QMenu* menuAnimations;

    MainWindow *_window;
};

#endif // MAINMENU_H
