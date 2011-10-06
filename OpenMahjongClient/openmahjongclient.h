#ifndef OPENMAHJONGCLIENT_H
#define OPENMAHJONGCLIENT_H

#include <QWidget>
#include <QMetaType>
#include "ClientQObject.h"
#include "common/ClientListener.h"


namespace Ui {
    class OpenMahjongClient;
}

class OpenMahjongClient : public QWidget,public OMClientListener
{
    Q_OBJECT

public:
    explicit OpenMahjongClient(QWidget *parent = 0);
    ~OpenMahjongClient();
public slots:
    void appendMessageText(OMString *);
    void enableCommand(OMCommand *);
    void onStarted(int index, OMTaku *);
    void onStatusCode(int code);
    void onDahaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void onTehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);
    void onTehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai *pai);

private slots:
    void on_m_btnConnect_clicked();

private:
    Ui::OpenMahjongClient *ui;
    OMClientQObject m_client;
    void layoutDahai(OMPai &pai,int index,int num);
    void layoutTehai(OMPai &pai,int index);

};

Q_DECLARE_METATYPE(OMTaku*)
Q_DECLARE_METATYPE(OMMember*)
Q_DECLARE_METATYPE(OMPai*)
Q_DECLARE_METATYPE(OMNakiMentsu*)


#endif // OPENMAHJONGCLIENT_H
