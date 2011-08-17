#include <QNetworkRequest>
#include <QNetworkReply>
#include "ClientQObject.h"
#include "ClientThread.h"

OMClientQObject::OMClientQObject(QObject *parent) :
    QObject(parent)
{
}

void OMClientQObject::sendString(QString &sendMessage, QString &recvMessage)
{
    OMClientThread *clThread = new OMClientThread();

    clThread->sendString(m_dstUrl,sendMessage,recvMessage);
}

void OMClientQObject::selectPai(OMPai &pai)
{

}

void OMClientQObject::run()
{
    gameSync();
}

void OMClientQObject::clientStart()
{

}

void OMClientQObject::clientStop()
{

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

}
