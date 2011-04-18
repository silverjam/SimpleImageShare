#-------------------------------------------------
#
# Project created by QtCreator 2011-04-18T11:43:18
#
#-------------------------------------------------

QT       += network

QT       -= gui

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

TARGET = SISLibrary
TEMPLATE = lib
CONFIG += staticlib

SOURCES += sislibrary.cpp \
    siscommands.cpp

HEADERS += sislibrary.h \
    siscommands.h \
    icommandsink.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
