#ifndef MAINMENU_H
#define MAINMENU_H

#include <QObject>
#include <QAction>
#include <QMenu>

// Forward declaration
class MainWindow;

// Holds the main menus and actions
class MainMenu : public QObject
{
    Q_OBJECT
public:
    explicit MainMenu(MainWindow* window);

signals:

private slots:
    // Slots corresponding to menu actions
    void newFile();
    void saveFile();
    void saveFileAs();

private:
    // Menu actions
    QAction* actNewFile;
    QAction* actSaveFile;
    QAction* actSaveAs;

    // Menus
    QMenu* menuFile;
    QMenu* menuEdit;
};

#endif // MAINMENU_H
