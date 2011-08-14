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
    OM_SYNC_STATE_STOP,
    OM_SYNC_STATE_ERROR
} OM_SYNC_STATE;

typedef enum {
    OM_GAME_STATE_STOP,
    OM_GAME_STATE_START
} OM_GAME_STATE;

class OMGenericClient
{
public:
    virtual ~OMGenericClient() {}
    OM_SYNC_STATE gameSync();
    void gameStart();
    void gameStop();
    void sendLibrary(OMLibrary&);
    void sendCommand(OM_DEFARRAY(OMCommand)&,QString&);
    int sendCommand(OMCommand&,QString&);
    void connect(OM_CONNECTION_TYPE contype,int session = 0);
    /* HTTPによりサーバーに文字列を送受信するメソッドをサブクラスで実装してください */
    virtual void sendString(QString& sendMessage,QString& recvMessage) = 0;
    void setClientListener(OMClientListener *pListener);
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
    BOOL m_bFirst;
    int m_iCurState;
    OMPlayer m_players[4];
    OMPlayer *m_pCurPlayer;
    OMClientListener *m_pListener;
    OM_GAME_STATE m_gamestate;



};

#endif // GENERICCLIENT_H
