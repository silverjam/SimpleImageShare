#-------------------------------------------------
#
# Project created by QtCreator 2011-04-18T11:54:28
#
#-------------------------------------------------

QT       += network testlib

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

LIBS += $${DESTDIR}/SISLibrary.lib

INCLUDEPATH += ../SISLibrary

TARGET = tst_sislibrarytest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_POST_LINK = $${DESTDIR}/$${TARGET}

SOURCES += tst_sislibrarytest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
