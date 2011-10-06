#ifndef CLIENTQOBJECT_H
#define CLIENTQOBJECT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QThread>
#include <QTimer>
#include "common/GenericClient.h"
#include "common/ClientListener.h"
#include "common/TakuListener.h"
#include "common/Pai.h"

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
    void setDestination(QUrl &url);
    virtual void tehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void tehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void dahaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void dahaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void dahaiNaki(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void nakiAdded(OMTaku *taku,int memberIndex,OMMember *member,OMNakiMentsu *mentsu);
    void nakiRemoved(OMTaku *taku,int memberIndex,OMMember *member,OMNakiMentsu *mentsu);
    void appendMessageText(OMString *);
    void enableCommand(OMCommand*);
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
private:
    QUrl m_dstUrl;
    QNetworkAccessManager m_naManager;
    QWidget *m_pTakuWidget;
    QThread m_thread;
    QTimer *m_pTimer;


signals:
    void waitingUserCommand();
    void sigTehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void sigTehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void sigDahaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void sigDahaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void sigDahaiNaki(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void sigNakiAdded(OMTaku *taku,int memberIndex,OMMember *member,OMNakiMentsu *mentsu);
    void sigNakiRemoved(OMTaku *taku,int memberIndex,OMMember *member,OMNakiMentsu *mentsu);
    void sigAppendMessageText(OMString *);
    void sigEnableCommand(OMCommand*);
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

public slots:
    void clientStart();
    void clientStop();
    void selectPai(OMPai &pai);
    void confirmCommand();
    void takuUpdate();
private slots:
    void clientStartImpl();
    void clientStopImpl();
};

#endif // CLIENTQOBJECT_H
