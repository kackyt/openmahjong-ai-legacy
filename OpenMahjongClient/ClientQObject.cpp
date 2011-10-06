#include <QNetworkRequest>
#include <QNetworkReply>
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

    clThread->sendString(m_dstUrl,sendMessage,recvMessage);
}

void OMClientQObject::selectPai(OMPai &pai)
{

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

}

void OMClientQObject::setDestination(QUrl &url)
{
    m_dstUrl = url;
}

void OMClientQObject::takuUpdate()
{
    gameSync();
}

void OMClientQObject::tehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai)
{
    emit sigTehaiAdded(taku,memberIndex,member,paiIndex,pai);
}

void OMClientQObject::tehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai)
{
    emit sigTehaiRemoved(taku,memberIndex,member,paiIndex,pai);
}

void OMClientQObject::dahaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai)
{
    emit sigDahaiAdded(taku,memberIndex,member,paiIndex,pai);

}

void OMClientQObject::dahaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai)
{
    emit sigDahaiRemoved(taku,memberIndex,member,paiIndex,pai);

}

void OMClientQObject::dahaiNaki(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai)
{
    emit sigDahaiNaki(taku,memberIndex,member,paiIndex,pai);

}

void OMClientQObject::nakiAdded(OMTaku *taku,int memberIndex,OMMember *member,OMNakiMentsu *mentsu)
{
    emit sigNakiAdded(taku,memberIndex,member,mentsu);
}

void OMClientQObject::nakiRemoved(OMTaku *taku,int memberIndex,OMMember *member,OMNakiMentsu *mentsu)
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

void OMClientQObject::enableCommand(OMCommand *)
{

}

void OMClientQObject::clientStartImpl()
{
    /* タイマーのスタート */
    gameStart();
    m_pTimer = new QTimer(this);
    QObject::connect(m_pTimer,SIGNAL(timeout()),SLOT(takuUpdate()));
    m_pTimer->setInterval(100);
    m_pTimer->setSingleShot(false);
    m_pTimer->start();
}

void OMClientQObject::clientStopImpl()
{
    gameStop();
    m_pTimer->stop();
}
