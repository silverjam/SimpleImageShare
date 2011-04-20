#-------------------------------------------------
#
# Project created by QtCreator 2011-04-18T07:59:49
#
#-------------------------------------------------

QT       += core gui network

TARGET = SIS
TEMPLATE = app

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
    LIBS += \
        #$${DESTDIR}/SISLibrary.lib \
        #$${DESTDIR}/SISServerLibrary.lib \
}
else {
    LIBS += \
        #$${DESTDIR}/libSISLibrary.a \
        #$${DESTDIR}/libSISServerLibrary.a \
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    baboon.png

RESOURCES += \
    SIS.qrc
