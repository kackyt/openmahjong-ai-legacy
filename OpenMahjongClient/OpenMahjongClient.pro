#-------------------------------------------------
#
# Project created by QtCreator 2011-07-10T10:26:45
#
#-------------------------------------------------

QT       += core gui xml network

TARGET = OpenMahjongClient
TEMPLATE = app


SOURCES += main.cpp\
        openmahjongclient.cpp \
    common/NakiMentsu.cpp \
    common/Message.cpp \
    common/Mentsu.cpp \
    common/Member.cpp \
    common/Library.cpp \
    common/GameState.cpp \
    common/Command.cpp \
    common/TakuEvent.cpp \
    common/Taku.cpp \
    common/Rule.cpp \
    common/Result.cpp \
    common/Player.cpp \
    common/Pai.cpp \
    common/GenericClient.cpp \
    common/ClientListener.cpp \
    PaiButton.cpp \
    ConnectDialog.cpp \
    ClientQObject.cpp \
    common/Exception.cpp \
    common/ConnectionException.cpp \
    common/IllegalParamException.cpp \
    common/IllegalStateException.cpp \
    ClientThread.cpp

HEADERS  += openmahjongclient.h \
    common/OMXMLCommon.h \
    common/NakiMentsu.h \
    common/Message.h \
    common/Mentsu.h \
    common/Member.h \
    common/Library.h \
    common/GameState.h \
    common/Command.h \
    common/TakuEvent.h \
    common/Taku.h \
    common/Rule.h \
    common/Result.h \
    common/Player.h \
    common/Pai.h \
    common/OpenMahjongDef.h \
    common/MIPIface.h \
    common/GenericClient.h \
    common/ClientListener.h \
    PaiButton.h \
    ConnectDialog.h \
    ClientQObject.h \
    common/Exception.h \
    common/ConnectionException.h \
    common/IllegalParamException.h \
    common/IllegalStateException.h \
    ClientThread.h

FORMS    += openmahjongclient.ui \
    ConnectDialog.ui

RESOURCES += \
    OMResource.qrc
