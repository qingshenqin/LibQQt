QT -= gui

CONFIG += c++11
#CONFIG += console
#去掉console，调试的时候没有弹出命令窗口

CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp

#-------------------------------------------------
#link qqt library
#if you link a library to your app, on android you must select the running kit to the app, not LibQQt e.g.
#-------------------------------------------------
include(../../examples/qqtframe2/link_qqt_library.pri)

#-------------------------------------------------
#install
#-------------------------------------------------
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

#-------------------------------------------------
##project environ
#-------------------------------------------------
#default
message ($${TARGET} config $${CONFIG})
message ($${TARGET} define $${DEFINES})

contains(QKIT_PRIVATE, ANDROID|ANDROIDX86) {
    CONFIG += mobility
    MOBILITY =
    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}
