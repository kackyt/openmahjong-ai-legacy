#-------------------------------------------------
#
# Project created by QtCreator 2011-07-10T10:26:45
#
#-------------------------------------------------

QT       += core gui xml network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    ../XMLLib/ClientListener.cpp \
    ../XMLLib/Command.cpp \
    ../XMLLib/ConnectionException.cpp \
    ../XMLLib/Exception.cpp \
    ../XMLLib/GameState.cpp \
    ../XMLLib/GenericClient.cpp \
    ../XMLLib/IllegalParamException.cpp \
    ../XMLLib/IllegalStateException.cpp \
    ../XMLLib/Library.cpp \
    ../XMLLib/Member.cpp \
    ../XMLLib/Mentsu.cpp \
    ../XMLLib/Message.cpp \
    ../XMLLib/NakiMentsu.cpp \
    ../XMLLib/OMXMLCommonQt.cpp \
    ../XMLLib/Pai.cpp \
    ../XMLLib/Player.cpp \
    ../XMLLib/Result.cpp \
    ../XMLLib/Rule.cpp \
    ../XMLLib/Taku.cpp \
    ../XMLLib/TakuEvent.cpp \
    ../XMLLib/TakuListener.cpp \
    ../XMLLib/UICommander.cpp \
    ../AILib/AILib.c \
    ../AILib/MJ0.c \
    ../AILib/YakuCheck.c \
    ../XMLLib/StdAfx.cpp

HEADERS  += openmahjongclient.h \
    PaiButton.h \
    ConnectDialog.h \
    ClientQObject.h \
    ClientThread.h \
    AgariDialog.h \
    MentsuWidget.h \
    FilePath.h \
    ../XMLLib/ClientListener.h \
    ../XMLLib/Command.h \
    ../XMLLib/ConnectionException.h \
    ../XMLLib/Exception.h \
    ../XMLLib/GameState.h \
    ../XMLLib/GenericClient.h \
    ../XMLLib/IllegalParamException.h \
    ../XMLLib/IllegalStateException.h \
    ../XMLLib/Library.h \
    ../XMLLib/Member.h \
    ../XMLLib/Mentsu.h \
    ../XMLLib/Message.h \
    ../XMLLib/MIPIface.h \
    ../XMLLib/NakiMentsu.h \
    ../XMLLib/OMXMLCommon.h \
    ../XMLLib/OpenMahjongDef.h \
    ../XMLLib/Pai.h \
    ../XMLLib/Player.h \
    ../XMLLib/Result.h \
    ../XMLLib/Rule.h \
    ../XMLLib/Taku.h \
    ../XMLLib/TakuEvent.h \
    ../XMLLib/TakuListener.h \
    ../XMLLib/UICommander.h \
    ../AILib/AILib.h \
    ../AILib/MIPIface.h \
    ../AILib/MJ0.h \
    ../XMLLib/StdAfx.h

FORMS    += openmahjongclient.ui \
    ConnectDialog.ui \
    AgariDialog.ui

RESOURCES +=

DEFINES += WITHQT

INCLUDEPATH += ../AILib ../XMLLib

CODECFORTR = SJIS
CODECFORSRC = SJIS
TRANSLATIONS = OpenMahjong_en.ts

