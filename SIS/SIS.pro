#-------------------------------------------------
#
# Project created by QtCreator 2011-04-18T07:59:49
#
#-------------------------------------------------

QT       += core gui

TARGET = SIS
TEMPLATE = app

debug {
    DESTDIR = ../bin/debug
    OBJECTS_DIR = ../build/debug
    MOC_DIR = ../build/debug
    RCC_DIR = ../build/debug
    UI_DIR = ../build/debug
}
else:release {
    DESTDIR = ../bin/release
    OBJECTS_DIR = ../build/release
    MOC_DIR = ../build/release
    RCC_DIR = ../build/release
    UI_DIR = ../build/release
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    baboon.png

RESOURCES += \
    SIS.qrc
