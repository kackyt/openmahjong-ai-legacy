#ifndef CLIENTQOBJECT_H
#define CLIENTQOBJECT_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMap>
#include "GenericClient.h"
#include "ClientListener.h"
#include "TakuListener.h"
#include "Pai.h"
#include "UICommander.h"
#include "ClientThread.h"

class OMClientQObject : public QObject,public OMGenericClient,public OMTakuListener,public OMClientListener
{
    Q_OBJECT
    typedef enum {
        SELECT_STATE_SUTEHAI,
        SELECT_STATE_TII,
        SELECT_STATE_PON,
        SELECT_STATE_KAN,
        SELECT_STATE_RIICHI
    } SELECT_STATE;
public:
    explicit OMClientQObject(QObject *parent = 0);
    virtual ~OMClientQObject();
    QMap<OMString,QLibrary *> m_playerTemplate;
    OMUICommander m_commander;
    void setDestination(QUrl &url);
    void setComp(OMString *name,int size);
    virtual void tehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai,bool tsumo);
    void tehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai,bool tsumo);
    void dahaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai);
    void dahaiNaki(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai);
    void nakiAdded(OMTaku *taku,int memberIndex,OMMember *member,int mentsuIndex,OMNakiMentsu mentsu,bool kuwae);
    void nakiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int mentsuIndex,OMNakiMentsu mentsu);
    void appendMessageText(OMString *);
    void onTii();
    void onPon();
    void onKan();
    void onTsumo();
    void onRon();
    void onRiichi();
    void onRivalRiichi();
    void onDahai();
    void onProgressed(int index,OMTaku *);
    void onStarted(int index,OMTaku *);
    void onStatusCode(int code);
protected:
    void sendString(OMString &sendMessage, OMString &recvMessage);
    virtual void createCompInstance(OMPlayer &player);
private:
    QUrl m_dstUrl;
    OMClientThread m_clientThread;
    QWidget *m_pTakuWidget;
    QThread m_thread;
    QTimer *m_pTimer;
    int m_iSyncInterval;
    bool m_bMyTurnSync;


signals:
    void waitingUserCommand();
    void sigTehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai,bool tsumo);
    void sigTehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai,bool tsumo);
    void sigDahaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai);
    void sigDahaiNaki(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai);
    void sigNakiAdded(OMTaku *taku,int memberIndex,OMMember *member,int mentsuIndex,OMNakiMentsu mentsu,bool kuwae);
    void sigNakiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int mentsuIndex,OMNakiMentsu mentsu);
    void sigAppendMessageText(OMString *);
    void sigTii();
    void sigPon();
    void sigKan();
    void sigTsumo();
    void sigRon();
    void sigRiichi();
    void sigRivalRiichi();
    void sigDahai();
    void sigProgressed(int index,OMTaku *);
    void sigStarted(int index,OMTaku *);
    void sigStatusCode(int code);
    void sigResponceCode(int code);
    void sigUserTurn(OMTaku *);
    void sigKyokuEnd(OMString message,OMTaku *);
    void sigDisconnected();

public slots:
    void clientStart();
    void clientStop();
    void confirmCommand();
    void takuUpdate();
    void setSyncInterval(int interval);
    void setMyTurnSync(bool sync);
private slots:
    void clientStartImpl();
    void clientStopImpl();
};

#endif // CLIENTQOBJECT_H
