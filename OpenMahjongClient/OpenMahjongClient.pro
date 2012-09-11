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
    PaiButton.cpp \
    ConnectDialog.cpp \
    ClientQObject.cpp \
    ClientThread.cpp \
    AgariDialog.cpp \
    MentsuWidget.cpp \
    common/XMLLib/UICommander.cpp \
    common/XMLLib/TakuListener.cpp \
    common/XMLLib/TakuEvent.cpp \
    common/XMLLib/Taku.cpp \
    common/XMLLib/StdAfx.cpp \
    common/XMLLib/Rule.cpp \
    common/XMLLib/Result.cpp \
    common/XMLLib/Player.cpp \
    common/XMLLib/Pai.cpp \
    common/XMLLib/OMXMLCommonQt.cpp \
    common/XMLLib/NakiMentsu.cpp \
    common/XMLLib/Message.cpp \
    common/XMLLib/Mentsu.cpp \
    common/XMLLib/Member.cpp \
    common/XMLLib/Library.cpp \
    common/XMLLib/IllegalStateException.cpp \
    common/XMLLib/IllegalParamException.cpp \
    common/XMLLib/GenericClient.cpp \
    common/XMLLib/GameState.cpp \
    common/XMLLib/Exception.cpp \
    common/XMLLib/ConnectionException.cpp \
    common/XMLLib/Command.cpp \
    common/XMLLib/ClientListener.cpp \
    common/AILib/YakuCheck.c \
    common/AILib/MJ0.c \
    common/AILib/AILib.c

HEADERS  += openmahjongclient.h \
    PaiButton.h \
    ConnectDialog.h \
    ClientQObject.h \
    ClientThread.h \
    AgariDialog.h \
    MentsuWidget.h \
    FilePath.h \
    common/XMLLib/UICommander.h \
    common/XMLLib/TakuListener.h \
    common/XMLLib/TakuEvent.h \
    common/XMLLib/Taku.h \
    common/XMLLib/StdAfx.h \
    common/XMLLib/Rule.h \
    common/XMLLib/Result.h \
    common/XMLLib/Player.h \
    common/XMLLib/Pai.h \
    common/XMLLib/OpenMahjongDef.h \
    common/XMLLib/OMXMLCommon.h \
    common/XMLLib/NakiMentsu.h \
    common/XMLLib/MIPIface.h \
    common/XMLLib/Message.h \
    common/XMLLib/Mentsu.h \
    common/XMLLib/Member.h \
    common/XMLLib/Library.h \
    common/XMLLib/IllegalStateException.h \
    common/XMLLib/IllegalParamException.h \
    common/XMLLib/GenericClient.h \
    common/XMLLib/GameState.h \
    common/XMLLib/Exception.h \
    common/XMLLib/ConnectionException.h \
    common/XMLLib/Command.h \
    common/XMLLib/ClientListener.h \
    common/AILib/MJ0.h \
    common/AILib/AILib.h

FORMS    += openmahjongclient.ui \
    ConnectDialog.ui \
    AgariDialog.ui

RESOURCES +=

INCLUDEPATH += ./common/AILib ./common/XMLLib

CODECFORTR = SJIS
CODECFORSRC = SJIS
TRANSLATIONS = OpenMahjong_en.ts

