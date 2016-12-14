/* CONTRIBUTORS
 * Ben Merritt: Set up the first few menu items in the MainMenu constructor
 * Paden Rumsey: Implemented/debugged most of the file operations 
 *               (except export)
 * Max Gilmore: Added insert menu with animation submenu, no animations implemented yet
 */
#include "mainmenu.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <string>
#include <iostream>
#include <QDebug>

#include "mainwindow.h"
#include "tanIO.h"

extern Animation *animation;
std::string previousFile = "";

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

    actOpenFile = new QAction(tr("&Open"), this);
    actOpenFile->setShortcuts(QKeySequence::Open);
    actOpenFile->setStatusTip(tr("Open an animation"));
    connect(actOpenFile, &QAction::triggered, this, &MainMenu::openFile);

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
    
    //Unimplemented animation
    RainEffect = new QAction(tr("Rain"), this);
    RainEffect->setStatusTip(tr("Insert falling rain effect after current frame"));
    //connect(RainEffect, &QAction::triggered, this, &MainMenu::rainEffect);

    // Create menus and add to window.

    QMenuBar* mainMenu = window->menuBar();
    menuFile = mainMenu->addMenu(tr("File"));
    menuFile->addAction(actNewFile);
    menuFile->addAction(actOpenFile);
    menuFile->addAction(actSaveFile);
    menuFile->addAction(actSaveAs);
    menuFile->addAction(actExport);
    menuFile->addAction(actExit);

    menuEdit = mainMenu->addMenu(tr("Edit"));
    menuEdit->addAction(Cut);
    menuEdit->addAction(Copy);
    menuEdit->addAction(Paste);
    
    menuInsert = mainMenu->addMenu(tr("Insert"));
    menuAnimations = menuInsert->addMenu("Animations");
    menuAnimations->addAction(RainEffect);

    _window = window;

}

void MainMenu::newFile() {
    QMessageBox newFileBox;
    newFileBox.setText("<p align='center'>Do you want to save your current animation?</p>");
    QAbstractButton* noSave = (QAbstractButton *) newFileBox.addButton(tr("Don't Save"), QMessageBox::YesRole);
    newFileBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    int ret = newFileBox.exec();
    previousFile = "";

    if(newFileBox.clickedButton() == noSave) {
        delete animation;
        animation = new Animation;
        animation->AddFrame(25);        //add default frame of 25 seconds
        _window->setAnimation(animation);
    }
    else{
        switch(ret) {
        case QMessageBox::Save:{
            QString fileName = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(), "Project Files (*.pro)");
            if(fileName == NULL || fileName.isEmpty())
                return;          //the file returned is NULL so the cancel button was pressed (or empty)
            std::string stdFileName = fileName.toStdString();
            SaveProject(stdFileName, animation);
            delete animation;
            animation = new Animation;
            animation->AddFrame(25);    //add default frame of 25 seconds
            _window->setAnimation(animation);
            break;
        }
        case QMessageBox::Cancel:
            return;
            break;
        default:
            std::cout << "Something bad happened\n";
            break;
        }
    }
}

void MainMenu::saveFile() {
    if(previousFile.empty()){
        QString fileName = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(), "Project Files (*.pro)");
        if(fileName == NULL || fileName.isEmpty())
            return;          //the file returned is NULL so the cancel button was pressed (or empty)
        std::string stdFileName = fileName.toStdString();
        SaveProject(stdFileName, animation);
        previousFile = stdFileName;
    }
    else
        SaveProject(previousFile, animation);
}

void MainMenu::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(), "Project Files (*.pro)");
    if(fileName == NULL || fileName.isEmpty())
        return;          //the file returned is NULL so the cancel button was pressed (or empty)
    std::string stdFileName = fileName.toStdString();
    SaveProject(stdFileName, animation);
    previousFile = stdFileName;
}

void MainMenu::openFile() {
    QMessageBox newFileBox;
    QString fileName;
    newFileBox.setText("Do you want to save your current animation?");
    QAbstractButton* noSave = (QAbstractButton*) newFileBox.addButton("Don't Save", QMessageBox::YesRole);
    newFileBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    int ret = newFileBox.exec();

    if(newFileBox.clickedButton() == noSave){
        fileName = QFileDialog::getOpenFileName(0, "Open file", QDir::currentPath(), "Tan or Project Files (*.tan *.pro)");
        if(fileName == NULL || fileName.isEmpty())
            return;          //the file returned is NULL so the cancel button was pressed (or empty)
        delete animation;
        animation = new Animation;
        std::string stdFileName = fileName.toStdString();
        LoadTan(stdFileName, animation);
        previousFile = stdFileName;
        _window->setAnimation(animation);
        }
    else{
        switch(ret) {
        case QMessageBox::Save:{
            fileName = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(), "Project Files (*.pro)");
            if(fileName == NULL || fileName.isEmpty())
                return;          //the file returned is NULL so the cancel button was pressed (or empty)
            std::string stdFileName = fileName.toStdString();
            SaveProject(stdFileName, animation);
            delete animation;
            animation = new Animation;
            fileName = QFileDialog::getOpenFileName(0, "Open file", QDir::currentPath(), "Project Files (*.pro)");
            stdFileName = fileName.toStdString();
            LoadTan(stdFileName, animation);
            previousFile = stdFileName;
            _window->setAnimation(animation);
            break;
        }
        case QMessageBox::Cancel:
            return;
            break;
        default:
            std::cout << "Something bad happened\n";
            break;
        }
    }
}

void MainMenu::exportFile() {
    QString fileName = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(), "Tan Files (*.tan)");
    if(fileName == NULL || fileName.isEmpty())
        return;          //the file returned is NULL so the cancel button was pressed (or empty)
    std::string stdFileName = fileName.toStdString();
    SaveTan(stdFileName, animation);
    previousFile = stdFileName;
}
