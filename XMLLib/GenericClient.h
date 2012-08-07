#ifndef GENERICCLIENT_H
#define GENERICCLIENT_H
#include "OMXmlCommon.h"
#include "Library.h"
#include "Command.h"
#include "Taku.h"
#include "ClientListener.h"

typedef enum {
    OM_CONNECTION_TYPE_CREATE,
    OM_CONNECTION_TYPE_CONNECT,
    OM_CONNECTION_TYPE_RECONNECT,
    OM_CONNECTION_TYPE_DEBUG
} OM_CONNECTION_TYPE;

typedef enum {
    OM_SYNC_STATE_USERCOMMAND,
    OM_SYNC_STATE_NEXTKYOKU,
    OM_SYNC_STATE_OK,
    OM_SYNC_STATE_STOP,
    OM_SYNC_STATE_ERROR
} OM_SYNC_STATE;

typedef enum {
    OM_GAME_STATE_PLAYERSETNAME,
    OM_GAME_STATE_CONNECTED,
    OM_GAME_STATE_STOPED,
    OM_GAME_STATE_STARTED,
    OM_GAME_STATE_PROGRESSING,
    OM_GAME_STATE_WAITCOMMAND
} OM_GAME_STATE;

class OMGenericClient
{
public:
    OMGenericClient();
    virtual ~OMGenericClient() {}
    OM_SYNC_STATE gameSync();
    UINT MJSendMessage(void *inst,UINT message,UINT param1,UINT param2);
    void gameStart();
    void gameStop();
    void sendLibrary(OMLibrary&);
    void sendCommand(OMArray<OMCommand>&,OMString&);
    int sendCommand(OMCommand&,OMString&);
    void connect(OM_CONNECTION_TYPE contype,int session = 0);
    void setClientListener(OMClientListener *pListener);
    void setPlayerName(OMArray<OMString>&playernames,OMArray<OMString>&compnames);
    void getPlayerName(OMArray<OMString>&playernames) const;
    int getPlayerIndex() const;
    int getSessionNum() const;
private:
    void debugPrint();
protected:
    OMRule m_rule;
    BOOL m_bBusy;
    int m_iSession;
    int m_iPlayerNum;
    int m_iCompNum;
    int m_iPlayerIndex;
    int m_iSyncTick;
    OMTaku *m_pCurTaku;
    OMTaku m_aTakuAll[4];
    int m_iCurState;
    OMPlayer m_players[4];
    OMPlayer *m_pCurPlayer;
    OM_GAME_STATE m_gamestate;
    OMClientListener *m_pListener;
    /* HTTPによりサーバーに文字列を送受信するメソッドをサブクラスで実装してください */
    virtual void sendString(OMString& sendMessage,OMString& recvMessage) = 0;
    virtual void createCompInstance(OMPlayer &player) = 0;

};

#endif // GENERICCLIENT_H
