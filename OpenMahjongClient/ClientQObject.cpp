#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include "ClientQObject.h"
#include "ClientThread.h"

OMClientQObject::OMClientQObject(QObject *parent) :
    QObject(parent)
{
    int i;
    setClientListener(this);
    for(i=0;i<4;i++){
        m_aTakuAll[i].setTakuListener(this);
    }
    moveToThread(&m_thread);
    m_thread.start();
}

OMClientQObject::~OMClientQObject()
{
    m_thread.quit();
    m_thread.wait();
}

void OMClientQObject::sendString(OMString &sendMessage, OMString &recvMessage)
{
    OMClientThread *clThread = new OMClientThread();

    try {
        clThread->sendString(m_dstUrl,sendMessage,recvMessage);
    }catch(...){

    }
}

void OMClientQObject::clientStart()
{
    QMetaObject::invokeMethod(this,"clientStartImpl");
}

void OMClientQObject::clientStop()
{
    QMetaObject::invokeMethod(this,"clientStopImpl");
}

void OMClientQObject::confirmCommand()
{
    OMCommand com;
    OMString recvMessage;
    int res;

    qDebug() << "confirmCommand";

    m_commander.getCommand(com);

    com.m_player = *m_pCurPlayer;

    while((res = sendCommand(com,recvMessage)) < 0);

    qDebug() << "res = " << res;

    if(res != RESPONCE_OK){
        emit sigResponceCode(res);
    }
}

void OMClientQObject::setDestination(QUrl &url)
{
    m_dstUrl = url;
}

void OMClientQObject::takuUpdate()
{
    OM_SYNC_STATE state;
    int i;

    state = gameSync();
    if(state == OM_SYNC_STATE_USERCOMMAND){
        m_commander.initialize(m_pCurTaku->m_members[getPlayerIndex()]);
        emit sigUserTurn();
    }else if(state == OM_SYNC_STATE_NEXTKYOKU){
        OMString text,recvMessage;
        OMCommand com;
        m_commander.initialize(m_pCurTaku->m_members[getPlayerIndex()]);


        if(m_pCurTaku->m_event.m_command.m_iType == TYPE_KOUHAI){
            text = "荒牌平局";
        }else{
            for(i=0;i<m_pCurTaku->m_event.m_result.m_aYaku.size();i++){
                text += m_pCurTaku->m_event.m_result.m_aYaku[i];
            }

        }

        m_commander.setStart();
        m_commander.getCommand(com);


        QMessageBox::information(NULL,"終局",text);

        /* 次の局へ */

        sendCommand(com,recvMessage);

        gameStart();


    }
}

void OMClientQObject::tehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai)
{
    emit sigTehaiAdded(taku,memberIndex,member,paiIndex,pai);
}

void OMClientQObject::tehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai)
{
    emit sigTehaiRemoved(taku,memberIndex,member,paiIndex,pai);
}

void OMClientQObject::dahaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai)
{
    emit sigDahaiAdded(taku,memberIndex,member,paiIndex,pai);

}

void OMClientQObject::dahaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai)
{
    emit sigDahaiRemoved(taku,memberIndex,member,paiIndex,pai);

}

void OMClientQObject::dahaiNaki(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai)
{
    emit sigDahaiNaki(taku,memberIndex,member,paiIndex,pai);

}

void OMClientQObject::nakiAdded(OMTaku *taku,int memberIndex,OMMember *member,OMNakiMentsu mentsu)
{
    emit sigNakiAdded(taku,memberIndex,member,mentsu);
}

void OMClientQObject::nakiRemoved(OMTaku *taku,int memberIndex,OMMember *member,OMNakiMentsu mentsu)
{
    emit sigNakiRemoved(taku,memberIndex,member,mentsu);
}

void OMClientQObject::onDahai()
{
    emit sigDahai();
}

void OMClientQObject::onKan()
{
    emit sigKan();
}

void OMClientQObject::onPon()
{
    emit sigPon();
}

void OMClientQObject::onStarted(int index, OMTaku *taku)
{
    emit sigStarted(index,taku);
}

void OMClientQObject::onProgressed(int index, OMTaku *taku)
{
    emit sigProgressed(index,taku);
}

void OMClientQObject::onRiichi()
{
    emit sigRiichi();
}

void OMClientQObject::onRivalRiichi()
{
    emit sigRiichi();
}

void OMClientQObject::onRon()
{
    emit sigRon();
}

void OMClientQObject::onStatusCode(int code)
{
    emit sigStatusCode(code);
}

void OMClientQObject::onTii()
{
    emit sigTii();
}

void OMClientQObject::onTsumo()
{
    emit sigTsumo();
}

void OMClientQObject::appendMessageText(OMString *)
{

}

void OMClientQObject::clientStartImpl()
{
    /* タイマーのスタート */
    gameStart();
    m_pTimer = new QTimer(this);
    QObject::connect(m_pTimer,SIGNAL(timeout()),SLOT(takuUpdate()));
    m_pTimer->setInterval(3000);
    m_pTimer->setSingleShot(false);
    m_pTimer->start();
}

void OMClientQObject::clientStopImpl()
{
    gameStop();
    m_pTimer->stop();
}
