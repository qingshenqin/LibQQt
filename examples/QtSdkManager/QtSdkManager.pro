#-------------------------------------------------
#
# Project created by QtCreator 2017-10-08T23:30:20
#
#-------------------------------------------------
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QT += core gui network sql xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport serialport
greaterThan(QT_MAJOR_VERSION, 4): DEFINES += __QT5__

TARGET = qqtframe2
TEMPLATE = app

QKIT_ = $$(QKIT)
message($${QKIT_} Defined in $${TARGET})
equals(QKIT_, MIPS32) {
    QT += multimedia
    DEFINES += __EMBEDDED_LINUX__
} else:equals(QKIT_, LINUX) {
    DEFINES += __LINUX__
} else:equals(QKIT_, LINUX64) {
    DEFINES += __LINUX64__
} else:equals(QKIT_, WIN) {
    DEFINES += __WIN__
} else:equals(QKIT_, WIN64) {
    DEFINES += __WIN64__
} else:equals(QKIT_, macOS) {
    DEFINES += __DARWIN__
} else:equals(QKIT_, Android) {
    DEFINES += __ANDROID__
}
equals(QKIT_, macOS) {
    CONFIG += app_bundle
}

CONFIG(debug, debug|release) {
} else {
    DEFINES -= QT_NO_DEBUG_OUTPUT
}

equals(QKIT_, Android) {
    CONFIG += mobility
    MOBILITY =
    #DISTFILES += \
    #    android/AndroidManifest.xml

    #ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

INCLUDEPATH += $$PWD \
    $$PWD/../../src \
    $$PWD/../../src/core \
    $$PWD/../../src/gui \
    $$PWD/../../src/multimedia \
    $$PWD/../../src/network \
    $$PWD/../../src/printsupport \
    $$PWD/../../src/sql \
    $$PWD/../../src/widgets \
    $$PWD/../../src/customplot \
    $$PWD/../../src/pluginwatcher \
    $$PWD/../../src/dmmu \
    $$PWD/../../src/frame

#include(../../src/qqt.pri)
equals(QKIT_, macOS) {
    LIBS += -F/Users/abel/Develop/c0-buildstation/a0-qqtfoundation/MacOS/Release/src/bin
    LIBS += -framework QQt
}
equals(QKIT_, Android) {
    LIBS += -L/Users/abel/Develop/c0-buildstation/a0-qqtfoundation/Android-arm/Release/src/bin
    LIBS += -lQQt
}

############
##build cache
############
OBJECTS_DIR = obj
MOC_DIR = obj/moc.cpp
UI_DIR = obj/ui.h
RCC_DIR = qrc
DESTDIR = bin
############
##install
############
#CONFIG += can_install
can_install:equals(QKIT_, MIPS32) {
    target.path = /Application
    INSTALLS += target
} else: unix {
    equals(QKIT_, macOS){
        target.path = /Applications
        INSTALLS += target
    }
}

INCLUDEPATH +=  $$PWD

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
