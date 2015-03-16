#-------------------------------------------------
#
# Project created by QtCreator 2015-03-14T15:02:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gather
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treemodel.cpp \
    treeitem.cpp \
    infogetter.cpp

HEADERS  += mainwindow.h \
    treemodel.h \
    treeitem.h \
    infogetter.h

FORMS    += mainwindow.ui
