#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T10:00:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Terminus-Editor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

DISTFILES += \
    style.qss

RESOURCES += \
    style.qrc

Release:DESTDIR = ../../bin/editor/release
Release:OBJECTS_DIR = ../../bin/editor/release/.obj
Release:MOC_DIR = ../../bin/editor/release/.moc
Release:RCC_DIR = ../../bin/editor/release/.rcc
Release:UI_DIR = ../../bin/editor/release/.ui

Debug:DESTDIR = ../../bin/editor/debug
Debug:OBJECTS_DIR = ../../bin/editor/debug/.obj
Debug:MOC_DIR = ../../bin/editor/debug/.moc
Debug:RCC_DIR = ../../bin/editor/debug/.rcc
Debug:UI_DIR = ../../bin/editor/debug/.ui

