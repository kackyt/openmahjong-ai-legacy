#include <QNetworkRequest>
#include <QNetworkReply>
#include "ClientQObject.h"

OMClientQObject::OMClientQObject(QObject *parent) :
    QObject(parent)
{
}

void OMClientQObject::sendString(QString &sendMessage, QString &recvMessage)
{
    QNetworkRequest request;
    QNetworkReply *pReply;
    QByteArray recvByte;

    request.setUrl(m_dstUrl);

    pReply = m_naManager.post(request,sendMessage.toLocal8Bit());

    while(pReply->waitForReadyRead(-1)){
        recvByte += pReply->readAll();
    }

    recvMessage = QString::fromLocal8Bit(recvByte.constData());
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
