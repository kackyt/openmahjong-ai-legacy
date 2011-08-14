#ifndef OPENMAHJONGCLIENT_H
#define OPENMAHJONGCLIENT_H

#include <QWidget>
#include "ClientQObject.h"
#include "common/ClientListener.h"

namespace Ui {
    class OpenMahjongClient;
}

class OpenMahjongClient : public QWidget,OMClientListener
{
    Q_OBJECT

public:
    explicit OpenMahjongClient(QWidget *parent = 0);
    ~OpenMahjongClient();
    void appendMessageText(QString &);
    void enableCommand(OMCommand &);

private slots:
    void on_m_btnConnect_clicked();

private:
    Ui::OpenMahjongClient *ui;
    OMClientQObject m_client;

};

#endif // OPENMAHJONGCLIENT_H
