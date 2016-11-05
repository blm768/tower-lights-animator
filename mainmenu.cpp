#include "mainmenu.h"

#include <QMenuBar>

#include "mainwindow.h"

MainMenu::MainMenu(MainWindow* window) : QObject(window) {
    // Create actions.

    actNewFile = new QAction(tr("&New"), this);
    actNewFile->setShortcuts(QKeySequence::New);
    actNewFile->setStatusTip(tr("Create a new animation"));
    connect(actNewFile, &QAction::triggered, this, &MainMenu::newFile);

    actSaveFile = new QAction(tr("&Save"), this);
    actSaveFile->setShortcuts(QKeySequence::Save);
    actSaveFile->setStatusTip(tr("Save the animation"));
    connect(actSaveFile, &QAction::triggered, this, &MainMenu::saveFile);

    actSaveAs = new QAction(tr("&Save as"), this);
    actSaveAs->setShortcuts(QKeySequence::SaveAs);
    actSaveAs->setStatusTip(tr("Save the animation to a new file"));
    connect(actSaveAs, &QAction::triggered, this, &MainMenu::saveFileAs);

    // Create menus and add to window.

    QMenuBar* mainMenu = window->menuBar();
    menuFile = mainMenu->addMenu(tr("File"));
    menuFile->addAction(actNewFile);
    menuFile->addAction(actSaveFile);
    menuFile->addAction(actSaveAs);
}

void MainMenu::newFile() {

}

void MainMenu::saveFile() {

}

void MainMenu::saveFileAs() {

}
