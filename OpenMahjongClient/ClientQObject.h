#ifndef CLIENTQOBJECT_H
#define CLIENTQOBJECT_H

#include <QObject>
#include <QNetworkAccessManager>
#include "common/GenericClient.h"
#include "common/Pai.h"

class OMClientQObject : public QObject,public OMGenericClient,public QRunnable
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
    void sendString(QString &sendMessage, QString &recvMessage);
    void run();
private:
    QUrl m_dstUrl;
    QNetworkAccessManager m_naManager;


signals:
    void waitingUserCommand();

public slots:
    void clientStart();
    void clientStop();
    void selectPai(OMPai &pai);

};

#endif // CLIENTQOBJECT_H
