#-------------------------------------------------
#
# Project created by QtCreator 2012-08-16T22:28:23
#
#-------------------------------------------------

QT       -= core gui

TARGET = MahjongAI
TEMPLATE = lib

DEFINES += MAHJONGAI_LIBRARY

SOURCES += \
    MahjongAI.cpp

HEADERS += \
    MahjongAI_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEE2E3239
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = MahjongAI.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
