#-------------------------------------------------
#
# Project created by QtCreator 2011-04-18T11:43:18
#
#-------------------------------------------------

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

TARGET = SISLibrary
TEMPLATE = lib
CONFIG += staticlib

SOURCES += sislibrary.cpp \
    siscommands.cpp \
    commanddata.cpp \
    datapool.cpp

HEADERS += sislibrary.h \
    siscommands.h \
    icommandsink.h \
    commanddata.h \
    datapool.h \
    idatapoolsink.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
