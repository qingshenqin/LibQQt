#################################################################
##project QQt version pri
#################################################################
##qqt_library qqt_install all need this pri

QQT_MAJOR_VERSION = 1
QQT_MINOR_VERSION = 6
QQT_PATCH_VERSION = 0
QQT_BUILD_VERSION = 0
QQT_VERSION = $${QQT_MAJOR_VERSION}.$${QQT_MINOR_VERSION}.$${QQT_PATCH_VERSION}
QQT_VERSION4 = $${QQT_MAJOR_VERSION}.$${QQT_MINOR_VERSION}.$${QQT_PATCH_VERSION}.$${QQT_BUILD_VERSION}
message (Build QQt version: $$QQT_VERSION4)
