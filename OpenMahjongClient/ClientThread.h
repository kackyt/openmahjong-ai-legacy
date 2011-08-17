#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QUrl>
#include <QMutex>
#include <QWaitCondition>
#include <QNetworkReply>

class OMClientThread : public QThread
{
    Q_OBJECT
    typedef enum {
        OM_CLIENT_STATE_STOP,
        OM_CLIENT_STATE_STARTED,
        OM_CLIENT_STATE_ERROR
    } OM_CLIENT_STATE;
public:
    explicit OMClientThread(QObject *parent = 0);
    void sendString(QUrl &dst,QString &sendString,QString &recvString);
    void run();
private:
    QUrl m_dstUrl;
    QString m_sendString;
    QNetworkReply *m_pReply;
    QByteArray m_recvBuf;
    QMutex m_mutex;
    QWaitCondition m_wait;
    OM_CLIENT_STATE m_clientState;
signals:

private slots:
    void error(QNetworkReply::NetworkError);
    void readProgress();
    void finish();
};

#endif // CLIENTTHREAD_H
