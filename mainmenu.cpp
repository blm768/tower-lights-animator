#include "mainmenu.h"

#include "mainwindow.h"

MainMenu::MainMenu(MainWindow *window) : QObject(window) {
    actNewFile = new QAction(tr("&New"), this);
    actNewFile->setShortcuts(QKeySequence::New);
    actNewFile->setStatusTip(tr("Create a new animation"));
    connect(actNewFile, &QAction::triggered, this, &MainMenu::newFile);

    actSaveFile = new QAction(tr("&Save"), this);
    actSaveFile->setShortcuts(QKeySequence::Save);
    actSaveFile->setStatusTip(tr("Save the animation"));
    connect(actSaveFile, &QAction::triggered, this, &MainMenu::saveFile);

    // TODO: create menus and add to window.
}

void MainMenu::newFile() {

}

void MainMenu::saveFile() {

}
