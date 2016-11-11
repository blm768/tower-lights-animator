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

    actExport = new QAction(tr("&Export"), this);
    actExport->setStatusTip(tr("Export the animation to a tan file"));
    connect(actExport, &QAction::triggered, this, &MainMenu::exportFile);

    actExit = new QAction(tr("&Exit"), this);
    actExit->setShortcuts(QKeySequence::Quit);
    actExit->setStatusTip(tr("Exit the program"));
    connect(actExit, &QAction::triggered, this, &MainMenu::close);

    Cut = new QAction(tr("&Cut"), this);
    Cut->setShortcuts(QKeySequence::Cut);
    Cut->setStatusTip(tr("Cut selection"));
    connect(Cut, &QAction::triggered, this, &MainMenu::cut);

    Copy = new QAction(tr("&Copy"), this);
    Copy->setShortcuts(QKeySequence::Copy);
    Copy->setStatusTip(tr("Copy selection"));
    connect(Copy, &QAction::triggered, this, &MainMenu::copy);

    Paste = new QAction(tr("&Paste"), this);
    Paste->setShortcuts(QKeySequence::Paste);
    Paste->setStatusTip(tr("Paste from clipboard"));
    connect(Paste, &QAction::triggered, this, &MainMenu::paste);

    // Create menus and add to window.

    QMenuBar* mainMenu = window->menuBar();
    menuFile = mainMenu->addMenu(tr("File"));
    menuFile->addAction(actNewFile);
    menuFile->addAction(actSaveFile);
    menuFile->addAction(actSaveAs);
    menuFile->addAction(actExport);
    menuFile->addAction(actExit);

    menuEdit = mainMenu->addMenu(tr("Edit"));
    menuEdit->addAction(Cut);
    menuEdit->addAction(Copy);
    menuEdit->addAction(Paste);

}

void MainMenu::newFile() {

}

void MainMenu::saveFile() {

}

void MainMenu::saveFileAs() {

}

void MainMenu::exportFile() {

}
