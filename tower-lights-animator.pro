#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T15:10:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tower-lights-animator
TEMPLATE = app


SOURCES += main.cpp \
    animationmodel.cpp \
    mainwindow.cpp \
    timeline.cpp \
    frameeditor.cpp \
    frameregion.cpp \
    toolbar.cpp \
    mainmenu.cpp \
    colorpicker.cpp \
    towerframe.cpp \
    tanIO.cpp

HEADERS  += animationmodel.h \
    mainwindow.h \
    timeline.h \
    frameeditor.h \
    frameregion.h \
    toolbar.h \
    mainmenu.h \
    colorpicker.h \
    towerframe.h \
    tanIO.h
