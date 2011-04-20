#-------------------------------------------------
#
# Project created by QtCreator 2011-04-18T11:15:10
#
#-------------------------------------------------

QT       += core
QT       += network

QT       -= gui

debug {
    DESTDIR = ../bin/debug
    OBJECTS_DIR = ../build/debug/$${TARGET}
    MOC_DIR = ../build/debug/$${TARGET}
    RCC_DIR = ../build/debug/$${TARGET}
    UI_DIR = ../build/debug/$${TARGET}
}
else:release {
    DESTDIR = ../bin/release
    OBJECTS_DIR = ../build/release/$${TARGET}
    MOC_DIR = ../build/release/$${TARGET}
    RCC_DIR = ../build/release/$${TARGET}
    UI_DIR = ../build/release/$${TARGET}
}

win32 {
    LIBS += $${DESTDIR}/SISLibrary.lib $${DESTDIR}/SISServerLibrary.lib
}
else {
    LIBS += $${DESTDIR}/libSISLibrary.a $${DESTDIR}/libSISServerLibrary.a
}

INCLUDEPATH += \
    ../SISLibrary \
    ../SISServerLibrary

TARGET = SISServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
HEADERS +=
