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
    void onStarted(int index, OMTaku *);
    void onStatusCode(int code);
    void onDahaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai);
    void onTehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai);
    void onTehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai);

private slots:
    void onMyTurn();
    void onSelectPai(OMPai *pai,bool enable);
    void on_m_btnConnect_clicked();

    void on_m_btnTii_toggled(bool checked);

    void on_m_btnPon_toggled(bool checked);

    void on_m_btnKan_toggled(bool checked);

    void on_m_btnRon_clicked();

    void on_m_btnTsumo_clicked();

    void on_m_btnRiichi_toggled(bool checked);

    void on_m_btnPass_clicked();

    void on_m_btnTouhai_clicked();

    void on_m_btnDecide_clicked();

private:
    Ui::OpenMahjongClient *ui;
    OMClientQObject m_client;
    void layoutDahai(OMPai &pai,int index,int num);
    void layoutTehai(OMPai &pai,int index,int num);
    void endTurn();
};

Q_DECLARE_METATYPE(OMTaku*)
Q_DECLARE_METATYPE(OMMember*)
Q_DECLARE_METATYPE(OMPai*)
Q_DECLARE_METATYPE(OMPai)
Q_DECLARE_METATYPE(OMNakiMentsu)


#endif // OPENMAHJONGCLIENT_H
