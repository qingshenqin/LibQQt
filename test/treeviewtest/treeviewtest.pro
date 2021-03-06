#-------------------------------------------------
#
# Project created by QtCreator 2017-11-19T09:27:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = treeviewtest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

CONFIG += mobility
MOBILITY = 


#qmake_pre/post_link will work after source changed but not pro pri changed.
system("touch main.cpp")

#-------------------------------------------------
#link qqt library
#if you link a library to your app, on android you must select the running kit to the app, not LibQQt e.g.
#user can modify any infomation under this annotation
#-------------------------------------------------

#include manager
include(../../multi-link/multi-link/add_base_manager.pri)



#-------------------------------------------------
#user app may use these these settings prefertly
#-------------------------------------------------
#-------------------------------------------------
#install app
#-------------------------------------------------
#CONFIG += can_install
can_install:equals(QSYS_PRIVATE, Embedded) {
    target.path = /Application
    INSTALLS += target
} else: unix {
    equals(QSYS_PRIVATE, macOS) {
        target.path = /Applications
        INSTALLS += target
    }
}

############
##config defination
############
equals(QSYS_PRIVATE, macOS) {
    CONFIG += app_bundle
}

contains(QSYS_PRIVATE, Android|AndroidX86) {
    CONFIG += mobility
    MOBILITY =
    DISTFILES += \
        android/AndroidManifest.xml

    ANDROID_PACKAGE_SOURCE_DIR = $${PWD}/android
}

#???????????????????????????????????????
add_version (1,0,0,0)

#?????????App
#app???build???deploy
add_deploy()

#???????????????
#libQQt???sdk???build???deploy
add_dependent_manager(QQt)

add_deploy_config($$PWD/AppRoot)

#-------------------------------------------------
##project environ
#-------------------------------------------------
#default
message ($${TARGET} config $${CONFIG})
message ($${TARGET} define $${DEFINES})


