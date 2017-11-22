#-------------------------------------------------
#
# Project created by QtCreator 2016-06-17T10:03:52
#
#-------------------------------------------------

QT += core gui network sql xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qqtframe
TEMPLATE = app

INCLUDEPATH +=  $$PWD

SOURCES += $$PWD/main.cpp $$PWD/qqtapp.cpp $$PWD/qqtwindow.cpp

HEADERS  += $$PWD/qqtapp.h $$PWD/qqtwindow.h

FORMS    += $$PWD/qqtwindow.ui

include(../qqtframe2/link_qqt_library.pri)

############
##install
############
#CONFIG += can_install
can_install:equals(QKIT_PRIVATE, EMBEDDED) {
    target.path = /Application
    INSTALLS += target
} else: unix {
    equals(QKIT_PRIVATE, macOS) {
        target.path = /Applications
        INSTALLS += target
    }
}

equals(QKIT_PRIVATE, ANDROID) {
    CONFIG += mobility
    MOBILITY =
}

#################################################################
##project environ
#################################################################
#default
message ($${TARGET} config $${CONFIG})
message ($${TARGET} define $${DEFINES})
