#-------------------------------------------------
#
# Project created by QtCreator 2011-04-20T13:16:21
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

TARGET = SISServerLibrary

TEMPLATE = lib
CONFIG += staticlib

DEFINES += SISSERVERLIBRARY_LIBRARY

SOURCES += \
    sisserverlibrary.cpp \
    server.cpp \

HEADERS += \
    sisserverlibrary.h \
    SISServerLibrary_global.h \
    server.h \

INCLUDEPATH += \
    ../SISLibrary \

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3B96899
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = SISServerLibrary.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
