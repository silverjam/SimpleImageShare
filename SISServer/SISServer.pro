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

#LIBS += SISLibrary

TARGET = SISServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.h
