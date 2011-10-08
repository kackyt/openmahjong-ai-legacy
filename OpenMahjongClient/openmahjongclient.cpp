#include <QDebug>
#include <QMessageBox>
#include "openmahjongclient.h"
#include "PaiButton.h"
#include "ui_openmahjongclient.h"
#include "ConnectDialog.h"

OpenMahjongClient::OpenMahjongClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenMahjongClient)
{
    qRegisterMetaType<OMTaku*>();
    qRegisterMetaType<OMMember*>();
    qRegisterMetaType<OMNakiMentsu>();
    qRegisterMetaType<OMPai*>();
    qRegisterMetaType<OMPai>();
    ui->setupUi(this);

    /* レイアウト方向の変更 */
    ui->m_layout_dahai10->setDirection(QBoxLayout::BottomToTop);
    ui->m_layout_dahai11->setDirection(QBoxLayout::BottomToTop);
    ui->m_layout_dahai12->setDirection(QBoxLayout::BottomToTop);
    ui->m_layout_dahai20->setDirection(QBoxLayout::RightToLeft);
    ui->m_layout_dahai21->setDirection(QBoxLayout::RightToLeft);
    ui->m_layout_dahai22->setDirection(QBoxLayout::RightToLeft);
    ui->m_layout_tehai1->setDirection(QBoxLayout::BottomToTop);
    ui->m_layout_tehai2->setDirection(QBoxLayout::RightToLeft);

    QObject::connect(&m_client,SIGNAL(sigStarted(int,OMTaku*)),SLOT(onStarted(int,OMTaku*)));
    QObject::connect(&m_client,SIGNAL(sigDahaiAdded(OMTaku*,int,OMMember*,int,OMPai)),SLOT(onDahaiAdded(OMTaku*,int,OMMember*,int,OMPai)));
    QObject::connect(&m_client,SIGNAL(sigTehaiAdded(OMTaku*,int,OMMember*,int,OMPai)),SLOT(onTehaiAdded(OMTaku*,int,OMMember*,int,OMPai)));
    QObject::connect(&m_client,SIGNAL(sigTehaiRemoved(OMTaku*,int,OMMember*,int,OMPai)),SLOT(onTehaiRemoved(OMTaku*,int,OMMember*,int,OMPai)));
    QObject::connect(&m_client,SIGNAL(sigUserTurn()),SLOT(onMyTurn()));
}

OpenMahjongClient::~OpenMahjongClient()
{
    delete ui;
}

void OpenMahjongClient::on_m_btnConnect_clicked()
{
    ConnectDialog dialog(this);

    if(dialog.exec() == QDialog::Accepted){
        /* パラメータを設定して接続 */
        OMArray<OMString> playernames,compnames;
        QUrl url;
        OM_CONNECTION_TYPE conType;
        QString strNum;
        int session;
        dialog.getPlayerName(playernames);
        dialog.getCompName(compnames);
        m_client.setPlayerName(playernames,compnames);
        dialog.getUrl(url);
        m_client.setDestination(url);
        conType = dialog.getConnectionType();
        ((OMGenericClient*)&m_client)->connect(conType,dialog.getSessionNum());

        session = m_client.getSessionNum();

        strNum.setNum(session);
        ui->m_labelSession->setText(strNum);

        /* クライアントスレッドをスタートさせる */
        m_client.clientStart();
    }
}

void OpenMahjongClient::appendMessageText(OMString *)
{

}

void OpenMahjongClient::layoutDahai(OMPai &pai, int index,int num)
{
    OMPaiButton *btn = new OMPaiButton();
    btn->setEnabled(false);
    btn->setPai(pai,pai.m_bRiichi ? ((index + 1) & 3) : index);
    switch(index){
    case 0:
        if(num < 6){
            ui->m_layout_dahai00->addWidget(btn,Qt::AlignLeft | Qt::AlignBottom);
        }else if(num < 12){
            ui->m_layout_dahai01->addWidget(btn,0,Qt::AlignLeft | Qt::AlignBottom);
        }else{
            ui->m_layout_dahai02->addWidget(btn,0,Qt::AlignLeft | Qt::AlignBottom);
        }
        break;
    case 1:
        if(num < 6){
            ui->m_layout_dahai10->addWidget(btn,0,Qt::AlignRight | Qt::AlignBottom);
        }else if(num < 12){
            ui->m_layout_dahai11->addWidget(btn,0,Qt::AlignRight | Qt::AlignBottom);
        }else{
            ui->m_layout_dahai12->addWidget(btn,0,Qt::AlignRight | Qt::AlignBottom);
        }
        break;
    case 2:
        if(num < 6){
            ui->m_layout_dahai20->addWidget(btn,0,Qt::AlignRight | Qt::AlignTop);
        }else if(num < 12){
            ui->m_layout_dahai21->addWidget(btn,0,Qt::AlignRight | Qt::AlignTop);
        }else{
            ui->m_layout_dahai22->addWidget(btn,0,Qt::AlignRight | Qt::AlignTop);
        }
        break;
    case 3:
        if(num < 6){
            ui->m_layout_dahai30->addWidget(btn,0,Qt::AlignLeft | Qt::AlignTop);
        }else if(num < 12){
            ui->m_layout_dahai31->addWidget(btn,0,Qt::AlignLeft | Qt::AlignTop);
        }else{
            ui->m_layout_dahai32->addWidget(btn,0,Qt::AlignLeft | Qt::AlignTop);
        }
        break;
    default:
        break;
    }

}

void OpenMahjongClient::layoutTehai(OMPai &pai, int index,int num)
{
    OMPaiButton *btn = new OMPaiButton();
    btn->setEnabled(false);
    btn->setPai(pai,index);
    switch(index){
    case 0:
        ui->m_layout_tehai0->insertWidget(num,btn,0,Qt::AlignLeft | Qt::AlignBottom);
        break;
    case 1:
        ui->m_layout_tehai1->insertWidget(num,btn,0,Qt::AlignRight | Qt::AlignBottom);
        break;
    case 2:
        ui->m_layout_tehai2->insertWidget(num,btn,0,Qt::AlignRight | Qt::AlignTop);
        break;
    case 3:
        ui->m_layout_tehai3->insertWidget(num,btn,0,Qt::AlignLeft | Qt::AlignTop);
        break;
    default:
        break;
    }
}

void OpenMahjongClient::onStarted(int index, OMTaku *taku)
{
    if(index == 0){
        int i,j,ind;
        OMPai pai;
        OMPaiButton *btn;
        ind = m_client.getPlayerIndex();
        for(i=0;i<4;i++){
            OMMember *member = &taku->m_members[ind];

            /* 牌を置いていく */
            for(j=0;j<member->m_aDahai.size();j++){
                layoutDahai(member->m_aDahai[j],j,i);
            }

            if(member->m_aTehai.size() > 0){
                for(j=0;j<member->m_aTehai.size();j++){
                    layoutTehai(member->m_aTehai[j],i,j);
                }
            }else{
                OMPai nilPai;
                nilPai.m_iCategory = 0;
                nilPai.m_iNo = 0;
                for(j=0;j<13-member->m_gamestate.m_aNakiList.size()*3;j++){
                    layoutTehai(nilPai,i,j);
                }
            }

            ind = (ind + 1) & 3;
        }

        for(i=0;i<4;i++){
            /* 表ドラ */
            btn = new OMPaiButton();

            if(i<taku->m_aDora.size()){
                pai = taku->m_aDora[i];
            }else{
                pai.m_iCategory = 0;
                pai.m_iNo = 0;
            }

            btn->setPai(pai,0);
            ui->m_layout_dora->addWidget(btn,0,Qt::AlignLeft | Qt::AlignBottom);

            /* 裏ドラ */
            btn = new OMPaiButton();

            if(i<taku->m_aUradora.size()){
                pai = taku->m_aUradora[i];
            }else{
                pai.m_iCategory = 0;
                pai.m_iNo = 0;
            }

            btn->setPai(pai,0);
            ui->m_layout_uradora->addWidget(btn,0,Qt::AlignLeft | Qt::AlignBottom);
        }

    }

}

void OpenMahjongClient::onStatusCode(int code)
{

}

void OpenMahjongClient::onDahaiAdded(OMTaku *taku, int memberIndex, OMMember *member, int paiIndex, OMPai pai)
{
    int ind = m_client.getPlayerIndex();
    int cha = OMMember::getChaDistance(ind,memberIndex);

    layoutDahai(pai,cha,paiIndex);
}

void OpenMahjongClient::onTehaiAdded(OMTaku *taku, int memberIndex, OMMember *member, int paiIndex, OMPai pai)
{
    int ind = m_client.getPlayerIndex();
    int cha = OMMember::getChaDistance(ind,memberIndex);

    layoutTehai(pai,cha,paiIndex);
}

void OpenMahjongClient::onTehaiRemoved(OMTaku *taku, int memberIndex, OMMember *member, int paiIndex, OMPai pai)
{
    int ind = m_client.getPlayerIndex();
    int cha = OMMember::getChaDistance(ind,memberIndex);
    QBoxLayout *layout = NULL;

    switch(cha){
    case 0:
        layout = ui->m_layout_tehai0;
        break;
    case 1:
        layout = ui->m_layout_tehai1;
        break;
    case 2:
        layout = ui->m_layout_tehai2;
        break;
    case 3:
        layout = ui->m_layout_tehai3;
        break;
    default:
        break;
    }

    if(layout != NULL){
        QLayoutItem *pItem = layout->itemAt(paiIndex);
        pItem->widget()->deleteLater(); // 必要かどうかわからないけど。
        layout->removeItem(pItem);
    }
}

void OpenMahjongClient::onMyTurn()
{
    /* 自分のターンが回ってきた */
    int i;
    for(i=0;i<ui->m_layout_tehai0->count();i++){
        OMPaiButton *widget = (OMPaiButton *)ui->m_layout_tehai0->itemAt(i)->widget();
        widget->setEnabled(true);
        QObject::connect(widget,SIGNAL(selectPai(OMPai*,bool)),SLOT(onSelectPai(OMPai*,bool)));
    }
    ui->m_btnTii->setEnabled(m_client.m_commander.isTiiAvailable());
    ui->m_btnPon->setEnabled(m_client.m_commander.isPonAvailable());
    ui->m_btnKan->setEnabled(m_client.m_commander.isKanAvailable());
    ui->m_btnRon->setEnabled(m_client.m_commander.isRonAvailable());
    ui->m_btnTsumo->setEnabled(m_client.m_commander.isTsumoAvailable());
    ui->m_btnRiichi->setEnabled(m_client.m_commander.isRiichiAvailable());
    ui->m_btnPass->setEnabled(m_client.m_commander.isPassAvailable());
    ui->m_btnTouhai->setEnabled(m_client.m_commander.isTouhaiAvailable());
    ui->m_btnDecide->setEnabled(true);
}

void OpenMahjongClient::endTurn()
{
    int i;
    bool enable;

    for(i=0;i<ui->m_layout_tehai0->count();i++){
        OMPaiButton *widget = (OMPaiButton *)ui->m_layout_tehai0->itemAt(i)->widget();
        widget->setEnabled(false);
        widget->setChecked(false);
        QObject::disconnect(widget,SIGNAL(selectPai(OMPai*,bool)),this,SLOT(onSelectPai(OMPai*,bool)));
    }
    ui->m_btnTii->setEnabled(false);
    ui->m_btnPon->setEnabled(false);
    ui->m_btnKan->setEnabled(false);
    ui->m_btnRon->setEnabled(false);
    ui->m_btnTsumo->setEnabled(false);
    ui->m_btnRiichi->setEnabled(false);
    ui->m_btnPass->setEnabled(false);
    ui->m_btnTouhai->setEnabled(false);
    ui->m_btnDecide->setEnabled(false);

    ui->m_btnTii->setChecked(false);
    ui->m_btnPon->setChecked(false);
    ui->m_btnKan->setChecked(false);
    ui->m_btnRiichi->setChecked(false);

    enable = QMetaObject::invokeMethod(&m_client,"confirmCommand");

    qDebug() << "endTurn " << enable;
}

void OpenMahjongClient::onSelectPai(OMPai *pai, bool enable)
{
    if(enable){
        if(m_client.m_commander.addPai(*pai)){
            endTurn();
        }
    }else{
        m_client.m_commander.removePai(*pai);
    }
}

void OpenMahjongClient::on_m_btnTii_toggled(bool checked)
{
    m_client.m_commander.setTii(checked);
}

void OpenMahjongClient::on_m_btnPon_toggled(bool checked)
{
    m_client.m_commander.setPon(checked);
}

void OpenMahjongClient::on_m_btnKan_toggled(bool checked)
{
    m_client.m_commander.setKan(checked);
}

void OpenMahjongClient::on_m_btnRon_clicked()
{
    if(m_client.m_commander.setRon()){
        endTurn();
    }
}

void OpenMahjongClient::on_m_btnTsumo_clicked()
{
    if(m_client.m_commander.setTsumo()){
        endTurn();
    }
}

void OpenMahjongClient::on_m_btnRiichi_toggled(bool checked)
{
    m_client.m_commander.setRiichi(checked);
}

void OpenMahjongClient::on_m_btnPass_clicked()
{
    if(m_client.m_commander.setPass()){
        endTurn();
    }
}

void OpenMahjongClient::on_m_btnTouhai_clicked()
{
    if(m_client.m_commander.setTouhai()){
        endTurn();
    }
}

void OpenMahjongClient::on_m_btnDecide_clicked()
{
    if(m_client.m_commander.setConfirm()){
        endTurn();
    }else{
        QMessageBox::warning(this,"コマンド失敗","その牌では実行できません。");
    }

}
