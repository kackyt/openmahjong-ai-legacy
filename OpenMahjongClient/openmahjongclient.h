#ifndef OPENMAHJONGCLIENT_H
#define OPENMAHJONGCLIENT_H

#include <QWidget>
#include <QMetaType>
#include <QSound>
#include <QTranslator>
#include "ClientQObject.h"
#include "ClientListener.h"
#include "PaiButton.h"

#define TRANSLATES (2)

namespace Ui {
    class OpenMahjongClient;
}

class OpenMahjongClient : public QWidget
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
    void onTehaiAdded(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai,bool tsumo);
    void onTehaiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai,bool tsumo);
    void onDahaiNaki(OMTaku *taku,int memberIndex,OMMember *member,int paiIndex,OMPai pai);
    void onNakiAdded(OMTaku *taku,int memberIndex,OMMember *member,int mentsuIndex,OMNakiMentsu mentsu,bool kuwae);
    void onNakiRemoved(OMTaku *taku,int memberIndex,OMMember *member,int mentsuIndex,OMNakiMentsu mentsu);
    void onProgressed(int index, OMTaku *);
    void onDisconnected();
    void onTii();
    void onDahai();
    void onPon();
    void onRiichi();

private slots:
    void onMyTurn(OMTaku *);
    void onSelectPai(OMPai *pai,bool enable);
    void onResponce(int code);
    void onKyokuEnd(OMString message,OMTaku *taku);
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

    void on_m_btnSyncUpdate_clicked();

    void on_m_cmbLanguage_activated(const QString &arg1);

    void on_m_cmbLanguage_activated(int index);

private:
    Ui::OpenMahjongClient *ui;
    OMClientQObject m_client;
    void layoutDahai(OMPai &pai,int index,int num);
    void removeDahai(int index,int num);
    void layoutTehai(OMPai &pai,int index,int num);
    void endTurn();
    void takuUpdate(OMTaku *taku);
    QVector<OMPaiButton *> m_aButton;
    QTranslator m_translator[TRANSLATES];

};

Q_DECLARE_METATYPE(OMTaku*)
Q_DECLARE_METATYPE(OMMember*)
Q_DECLARE_METATYPE(OMPai*)
Q_DECLARE_METATYPE(OMPai)
Q_DECLARE_METATYPE(OMNakiMentsu)
Q_DECLARE_METATYPE(OMString)


#endif // OPENMAHJONGCLIENT_H
