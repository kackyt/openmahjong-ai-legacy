#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QSound>
#include "FilePath.h"
#include "openmahjongclient.h"
#include "PaiButton.h"
#include "MentsuWidget.h"
#include "ui_openmahjongclient.h"
#include "ConnectDialog.h"

//static const QString ieStrTable[] = {QString("東家"),QString("南家"),QString("西家"),QString("北家")};

static const TCHAR *ieStrTable[] = {_T("東家"),_T("南家"),_T("西家"),_T("北家")};


OpenMahjongClient::OpenMahjongClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenMahjongClient)
{
    qRegisterMetaType<OMTaku*>();
    qRegisterMetaType<OMMember*>();
    qRegisterMetaType<OMNakiMentsu>();
    qRegisterMetaType<OMPai*>();
    qRegisterMetaType<OMPai>();
    qRegisterMetaType<OMString>();

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
    QObject::connect(&m_client,SIGNAL(sigDahaiNaki(OMTaku*,int,OMMember*,int,OMPai)),SLOT(onDahaiNaki(OMTaku*,int,OMMember*,int,OMPai)));
    QObject::connect(&m_client,SIGNAL(sigTehaiAdded(OMTaku*,int,OMMember*,int,OMPai,bool)),SLOT(onTehaiAdded(OMTaku*,int,OMMember*,int,OMPai,bool)));
    QObject::connect(&m_client,SIGNAL(sigTehaiRemoved(OMTaku*,int,OMMember*,int,OMPai,bool)),SLOT(onTehaiRemoved(OMTaku*,int,OMMember*,int,OMPai,bool)));
    QObject::connect(&m_client,SIGNAL(sigNakiAdded(OMTaku*,int,OMMember*,int,OMNakiMentsu,bool)),SLOT(onNakiAdded(OMTaku*,int,OMMember*,int,OMNakiMentsu,bool)));
    QObject::connect(&m_client,SIGNAL(sigUserTurn(OMTaku*)),SLOT(onMyTurn(OMTaku*)));
    QObject::connect(&m_client,SIGNAL(sigResponceCode(int)),SLOT(onResponce(int)));
    QObject::connect(&m_client,SIGNAL(sigKyokuEnd(OMString,OMTaku *)),SLOT(onKyokuEnd(OMString,OMTaku*)));
    QObject::connect(&m_client,SIGNAL(sigProgressed(int,OMTaku*)),SLOT(onProgressed(int,OMTaku*)));
    QObject::connect(&m_client,SIGNAL(sigDahai()),SLOT(onDahai()));
    QObject::connect(&m_client,SIGNAL(sigPon()),SLOT(onPon()));
    QObject::connect(&m_client,SIGNAL(sigTii()),SLOT(onTii()));
    QObject::connect(&m_client,SIGNAL(sigRiichi()),SLOT(onRiichi()));
    QObject::connect(ui->m_chkMySync,SIGNAL(toggled(bool)),&m_client,SLOT(setMyTurnSync(bool)));

}

OpenMahjongClient::~OpenMahjongClient()
{
    delete ui;
}

void OpenMahjongClient::on_m_btnConnect_clicked()
{
    ConnectDialog dialog(this);

    dialog.setClientQObject(m_client);

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
        try{
            ((OMGenericClient*)&m_client)->connect(conType,dialog.getSessionNum());

            session = m_client.getSessionNum();

            strNum.setNum(session);
            ui->m_labelSession->setText(strNum);

            /* クライアントスレッドをスタートさせる */
            m_client.clientStart();
        }catch(...){
            QMessageBox::warning(this,"接続失敗","接続に失敗しました。");
        }
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
            ui->m_layout_dahai00->insertWidget(ui->m_layout_dahai00->count()-1,btn,Qt::AlignLeft | Qt::AlignBottom);
        }else if(num < 12){
            ui->m_layout_dahai01->insertWidget(ui->m_layout_dahai01->count()-1,btn,0,Qt::AlignLeft | Qt::AlignBottom);
        }else{
            ui->m_layout_dahai02->insertWidget(ui->m_layout_dahai02->count()-1,btn,0,Qt::AlignLeft | Qt::AlignBottom);
        }
        break;
    case 1:
        if(num < 6){
            ui->m_layout_dahai10->insertWidget(ui->m_layout_dahai10->count()-1,btn,0,Qt::AlignRight | Qt::AlignBottom);
        }else if(num < 12){
            ui->m_layout_dahai11->insertWidget(ui->m_layout_dahai11->count()-1,btn,0,Qt::AlignRight | Qt::AlignBottom);
        }else{
            ui->m_layout_dahai12->insertWidget(ui->m_layout_dahai12->count()-1,btn,0,Qt::AlignRight | Qt::AlignBottom);
        }
        break;
    case 2:
        if(num < 6){
            ui->m_layout_dahai20->insertWidget(ui->m_layout_dahai20->count()-1,btn,0,Qt::AlignRight | Qt::AlignTop);
        }else if(num < 12){
            ui->m_layout_dahai21->insertWidget(ui->m_layout_dahai21->count()-1,btn,0,Qt::AlignRight | Qt::AlignTop);
        }else{
            ui->m_layout_dahai22->insertWidget(ui->m_layout_dahai22->count()-1,btn,0,Qt::AlignRight | Qt::AlignTop);
        }
        break;
    case 3:
        if(num < 6){
            ui->m_layout_dahai30->insertWidget(ui->m_layout_dahai30->count()-1,btn,0,Qt::AlignLeft | Qt::AlignTop);
        }else if(num < 12){
            ui->m_layout_dahai31->insertWidget(ui->m_layout_dahai31->count()-1,btn,0,Qt::AlignLeft | Qt::AlignTop);
        }else{
            ui->m_layout_dahai32->insertWidget(ui->m_layout_dahai32->count()-1,btn,0,Qt::AlignLeft | Qt::AlignTop);
        }
        break;
    default:
        break;
    }

}

void OpenMahjongClient::removeDahai(int index,int num)
{
    QLayoutItem *item;
    int idx;

    switch(index){
    case 0:
        if(num < 6){
            idx = ui->m_layout_dahai00->count()-2;
            item = ui->m_layout_dahai00->itemAt(idx);
            ui->m_layout_dahai00->removeItem(item);
            item->widget()->deleteLater();
        }else if(num < 12){
            idx = ui->m_layout_dahai01->count()-2;
            item = ui->m_layout_dahai01->itemAt(idx);
            ui->m_layout_dahai01->removeItem(item);
            item->widget()->deleteLater();
        }else{
            idx = ui->m_layout_dahai02->count()-2;
            item = ui->m_layout_dahai02->itemAt(idx);
            ui->m_layout_dahai02->removeItem(item);
            item->widget()->deleteLater();
        }
        break;
    case 1:
        if(num < 6){
            idx = ui->m_layout_dahai10->count()-2;
            item = ui->m_layout_dahai10->itemAt(idx);
            ui->m_layout_dahai10->removeItem(item);
            item->widget()->deleteLater();
        }else if(num < 12){
            idx = ui->m_layout_dahai11->count()-2;
            item = ui->m_layout_dahai11->itemAt(idx);
            ui->m_layout_dahai11->removeItem(item);
            item->widget()->deleteLater();
        }else{
            idx = ui->m_layout_dahai12->count()-2;
            item = ui->m_layout_dahai12->itemAt(idx);
            ui->m_layout_dahai12->removeItem(item);
            item->widget()->deleteLater();
        }
        break;
    case 2:
        if(num < 6){
            idx = ui->m_layout_dahai20->count()-2;
            item = ui->m_layout_dahai20->itemAt(idx);
            ui->m_layout_dahai20->removeItem(item);
            item->widget()->deleteLater();
        }else if(num < 12){
            idx = ui->m_layout_dahai21->count()-2;
            item = ui->m_layout_dahai21->itemAt(idx);
            ui->m_layout_dahai21->removeItem(item);
            item->widget()->deleteLater();
        }else{
            idx = ui->m_layout_dahai22->count()-2;
            item = ui->m_layout_dahai22->itemAt(idx);
            ui->m_layout_dahai22->removeItem(item);
            item->widget()->deleteLater();
        }
        break;
    case 3:
        if(num < 6){
            idx = ui->m_layout_dahai30->count()-2;
            item = ui->m_layout_dahai30->itemAt(idx);
            ui->m_layout_dahai30->removeItem(item);
            item->widget()->deleteLater();
        }else if(num < 12){
            idx = ui->m_layout_dahai31->count()-2;
            item = ui->m_layout_dahai31->itemAt(idx);
            ui->m_layout_dahai31->removeItem(item);
            item->widget()->deleteLater();
        }else{
            idx = ui->m_layout_dahai32->count()-2;
            item = ui->m_layout_dahai32->itemAt(idx);
            ui->m_layout_dahai32->removeItem(item);
            item->widget()->deleteLater();
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
        m_aButton.append(btn);
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
        QLayoutItem *item;

        /* レイアウトのクリーニング */
        while(ui->m_layout_dahai00->count() > 0){
            item = ui->m_layout_dahai00->itemAt(0);
            ui->m_layout_dahai00->removeItem(item);
            if(item != ui->m_spacer_d00){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai01->count() > 0){
            item = ui->m_layout_dahai01->itemAt(0);
            ui->m_layout_dahai01->removeItem(item);
            if(item != ui->m_spacer_d01){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai02->count() > 0){
            item = ui->m_layout_dahai02->itemAt(0);
            ui->m_layout_dahai02->removeItem(item);
            if(item != ui->m_spacer_d02){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai10->count() > 0){
            item = ui->m_layout_dahai10->itemAt(0);
            ui->m_layout_dahai10->removeItem(item);
            if(item != ui->m_spacer_d10){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai11->count() > 0){
            item = ui->m_layout_dahai11->itemAt(0);
            ui->m_layout_dahai11->removeItem(item);
            if(item != ui->m_spacer_d11){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai12->count() > 0){
            item = ui->m_layout_dahai12->itemAt(0);
            ui->m_layout_dahai12->removeItem(item);
            if(item != ui->m_spacer_d12){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai20->count() > 0){
            item = ui->m_layout_dahai20->itemAt(0);
            ui->m_layout_dahai20->removeItem(item);
            if(item != ui->m_spacer_d20){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai21->count() > 0){
            item = ui->m_layout_dahai21->itemAt(0);
            ui->m_layout_dahai21->removeItem(item);
            if(item != ui->m_spacer_d21){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai22->count() > 0){
            item = ui->m_layout_dahai22->itemAt(0);
            ui->m_layout_dahai22->removeItem(item);
            if(item != ui->m_spacer_d22){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai30->count() > 0){
            item = ui->m_layout_dahai30->itemAt(0);
            ui->m_layout_dahai30->removeItem(item);
            if(item != ui->m_spacer_d30){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai31->count() > 0){
            item = ui->m_layout_dahai31->itemAt(0);
            ui->m_layout_dahai31->removeItem(item);
            if(item != ui->m_spacer_d31){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dahai32->count() > 0){
            item = ui->m_layout_dahai32->itemAt(0);
            ui->m_layout_dahai32->removeItem(item);
            if(item != ui->m_spacer_d32){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_tehai0->count() > 0){
            item = ui->m_layout_tehai0->itemAt(0);
            ui->m_layout_tehai0->removeItem(item);
            if(item != ui->m_spacer_t0){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_tehai1->count() > 0){
            item = ui->m_layout_tehai1->itemAt(0);
            ui->m_layout_tehai1->removeItem(item);
            if(item != ui->m_spacer_t1){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_tehai2->count() > 0){
            item = ui->m_layout_tehai2->itemAt(0);
            ui->m_layout_tehai2->removeItem(item);
            if(item != ui->m_spacer_t2){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_tehai3->count() > 0){
            item = ui->m_layout_tehai3->itemAt(0);
            ui->m_layout_tehai3->removeItem(item);
            if(item != ui->m_spacer_t3){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_dora->count() > 0){
            item = ui->m_layout_dora->itemAt(0);
            ui->m_layout_dora->removeItem(item);
            if(item != ui->m_spacer_dora){
                item->widget()->deleteLater();
            }
        }
        while(ui->m_layout_uradora->count() > 0){
            item = ui->m_layout_uradora->itemAt(0);
            ui->m_layout_uradora->removeItem(item);
            if(item != ui->m_spacer_ura){
                item->widget()->deleteLater();
            }
        }

        ui->m_layout_dahai00->addItem(ui->m_spacer_d00);
        ui->m_layout_dahai01->addItem(ui->m_spacer_d01);
        ui->m_layout_dahai02->addItem(ui->m_spacer_d02);
        ui->m_layout_dahai10->addItem(ui->m_spacer_d10);
        ui->m_layout_dahai11->addItem(ui->m_spacer_d11);
        ui->m_layout_dahai12->addItem(ui->m_spacer_d12);
        ui->m_layout_dahai20->addItem(ui->m_spacer_d20);
        ui->m_layout_dahai21->addItem(ui->m_spacer_d21);
        ui->m_layout_dahai22->addItem(ui->m_spacer_d22);
        ui->m_layout_dahai30->addItem(ui->m_spacer_d30);
        ui->m_layout_dahai31->addItem(ui->m_spacer_d31);
        ui->m_layout_dahai32->addItem(ui->m_spacer_d32);

        ui->m_layout_tehai0->addItem(ui->m_spacer_t01);
        ui->m_layout_tehai0->addItem(ui->m_spacer_t0);
        ui->m_layout_tehai1->addItem(ui->m_spacer_t11);
        ui->m_layout_tehai1->addItem(ui->m_spacer_t1);
        ui->m_layout_tehai2->addItem(ui->m_spacer_t21);
        ui->m_layout_tehai2->addItem(ui->m_spacer_t2);
        ui->m_layout_tehai3->addItem(ui->m_spacer_t31);
        ui->m_layout_tehai3->addItem(ui->m_spacer_t3);

        ui->m_layout_dora->addItem(ui->m_spacer_dora);
        ui->m_layout_uradora->addItem(ui->m_spacer_ura);

        m_aButton.clear();

        ind = m_client.getPlayerIndex();
        for(i=0;i<4;i++){
            OMMember *member = &taku->m_members[ind];
            QLabel *pLabelName,*pLabelPoint;
            QString label;

            /* プレーヤー名と自風、得点 */
            switch(i){
            case 0:
                pLabelName = ui->label_name0;
                pLabelPoint = ui->label_point0;
                break;
            case 1:
                pLabelName = ui->label_name1;
                pLabelPoint = ui->label_point1;
                break;
            case 2:
                pLabelName = ui->label_name2;
                pLabelPoint = ui->label_point2;
                break;
            case 3:
            default:
                pLabelName = ui->label_name3;
                pLabelPoint = ui->label_point3;
                break;
            }

            label = QString("%1 %2").arg(member->m_player.m_strName).arg(ieStrTable[member->m_gamestate.m_iZikaze-1]);
            pLabelName->setText(label);
            label = QString("%1点").arg(member->m_iPoint);
            pLabelPoint->setText(label);

            /* 牌を置いていく */
            for(j=0;j<member->m_aDahai.size();j++){
                layoutDahai(member->m_aDahai[j],j,i);
            }

            if(member->m_aTehai.size() > 0){
                for(j=0;j<member->m_aTehai.size()-1;j++){
                    layoutTehai(member->m_aTehai[j],i,j);
                }

                if(member->m_gamestate.m_bTsumo){
                    layoutTehai(member->m_aTehai[j],i,j+1);
                }else{
                    layoutTehai(member->m_aTehai[j],i,j);
                }


            }else{
                OMPai nilPai;
                nilPai.m_iCategory = 0;
                nilPai.m_iNo = 0;
                for(j=0;j<13-member->m_gamestate.m_aNakiList.size()*3;j++){
                    layoutTehai(nilPai,i,j);
                }

                if(member->m_gamestate.m_bTsumo){
                    layoutTehai(nilPai,i,j+1);
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
            ui->m_layout_dora->insertWidget(i,btn,0,Qt::AlignLeft | Qt::AlignBottom);

            /* 裏ドラ */
            btn = new OMPaiButton();

            if(i<taku->m_aUradora.size()){
                pai = taku->m_aUradora[i];
            }else{
                pai.m_iCategory = 0;
                pai.m_iNo = 0;
            }

            btn->setPai(pai,0);
            ui->m_layout_uradora->insertWidget(i,btn,0,Qt::AlignLeft | Qt::AlignBottom);
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

    layoutDahai(pai,cha,member->getDahaiRealPos(pai));
}

void OpenMahjongClient::onTehaiAdded(OMTaku *taku, int memberIndex, OMMember *member, int paiIndex, OMPai pai,bool tsumo)
{
    int ind = m_client.getPlayerIndex();
    int cha = OMMember::getChaDistance(ind,memberIndex);

    if(tsumo){
        paiIndex++;
    }

    layoutTehai(pai,cha,paiIndex);
}

void OpenMahjongClient::onTehaiRemoved(OMTaku *taku, int memberIndex, OMMember *member, int paiIndex, OMPai pai,bool tsumo)
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

    if(tsumo){
        paiIndex++;
    }

    if(layout != NULL){
        QLayoutItem *pItem = layout->itemAt(paiIndex);
        layout->removeItem(pItem);
        if(cha == 0){
            m_aButton.remove(m_aButton.indexOf((OMPaiButton*)pItem->widget()),1);
        }
        pItem->widget()->deleteLater();
    }
}

void OpenMahjongClient::onMyTurn(OMTaku *taku)
{
    /* 自分のターンが回ってきた */
    int i;

    takuUpdate(taku);
    for(i=0;i<m_aButton.size();i++){
        OMPaiButton *widget = m_aButton[i];
        if(widget != NULL){
            widget->setEnabled(true);
            widget->setCheckable(true);
            QObject::connect(widget,SIGNAL(selectPai(OMPai*,bool)),SLOT(onSelectPai(OMPai*,bool)));
        }
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

    QSound::play(getFilePath("wav/command.wav"));
}

void OpenMahjongClient::endTurn()
{
    int i;
    bool enable;

    for(i=0;i<m_aButton.size();i++){
        OMPaiButton *widget = m_aButton[i];
        if(widget != NULL){
            widget->setEnabled(false);
            widget->setChecked(false);
            QObject::disconnect(widget,SIGNAL(selectPai(OMPai*,bool)),this,SLOT(onSelectPai(OMPai*,bool)));
        }
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

void OpenMahjongClient::onResponce(int code)
{
}

void OpenMahjongClient::onKyokuEnd(OMString message,OMTaku *taku)
{
    int ind = m_client.getPlayerIndex();
    int i;
    OMArray<OMPai> *pTehai;
    QLayoutItem *item;
    OMArray<OMNakiMentsu> *pNakiList;

    /* 手牌をレイアウト */
    pTehai = &taku->m_members[(ind + 1) % 4].m_aTehai;
    pNakiList = &taku->m_members[(ind + 1) % 4].m_gamestate.m_aNakiList;
    if(pTehai->size() != 0){
        while(ui->m_layout_tehai1->count() > 0){
            item = ui->m_layout_tehai1->itemAt(0);
            ui->m_layout_tehai1->removeItem(item);
            if(item != ui->m_spacer_t1){
                delete item->widget();
            }
        }
        ui->m_layout_tehai1->addItem(ui->m_spacer_t1);
        for(i=0;i<pTehai->size();i++){
            layoutTehai((*pTehai)[i],1,i);
        }

        for(i=0;i<pNakiList->size();i++){
            OMMentsuWidget *widget = new OMMentsuWidget();
            widget->setMentsu((*pNakiList)[i],1,OMMember::getChaDistance(ind,(*pNakiList)[i].m_iAite));
            ui->m_layout_tehai1->insertWidget(ui->m_layout_tehai1->count() - 1 - i,widget);
        }
    }
    pTehai = &taku->m_members[(ind + 2) % 4].m_aTehai;
    pNakiList = &taku->m_members[(ind + 2) % 4].m_gamestate.m_aNakiList;
    if(pTehai->size() != 0){
        while(ui->m_layout_tehai2->count() > 0){
            item = ui->m_layout_tehai2->itemAt(0);
            ui->m_layout_tehai2->removeItem(item);
            if(item != ui->m_spacer_t2){
                delete item->widget();
            }
        }
        ui->m_layout_tehai2->addItem(ui->m_spacer_t2);
        for(i=0;i<pTehai->size();i++){
            layoutTehai((*pTehai)[i],2,i);
        }
        for(i=0;i<pNakiList->size();i++){
            OMMentsuWidget *widget = new OMMentsuWidget();
            widget->setMentsu((*pNakiList)[i],2,OMMember::getChaDistance(ind,(*pNakiList)[i].m_iAite));
            ui->m_layout_tehai1->insertWidget(ui->m_layout_tehai1->count() - 1 - i,widget);
        }
    }
    pTehai = &taku->m_members[(ind + 3) % 4].m_aTehai;
    pNakiList = &taku->m_members[(ind + 3) % 4].m_gamestate.m_aNakiList;
    if(pTehai->size() != 0){
        while(ui->m_layout_tehai3->count() > 0){
            item = ui->m_layout_tehai3->itemAt(0);
            ui->m_layout_tehai3->removeItem(item);
            if(item != ui->m_spacer_t3){
                delete item->widget();
            }
        }
        ui->m_layout_tehai3->addItem(ui->m_spacer_t3);
        for(i=0;i<pTehai->size();i++){
            layoutTehai((*pTehai)[i],3,i);
        }
        for(i=0;i<pNakiList->size();i++){
            OMMentsuWidget *widget = new OMMentsuWidget();
            widget->setMentsu((*pNakiList)[i],3,OMMember::getChaDistance(ind,(*pNakiList)[i].m_iAite));
            ui->m_layout_tehai1->insertWidget(ui->m_layout_tehai1->count() - 1 - i,widget);
        }
    }

    /* 裏ドラ */
    if(taku->m_aUradora.size() != 0){
        OMPaiButton *btn;
        OMPai pai;
        while(ui->m_layout_uradora->count() > 0){
            item = ui->m_layout_uradora->itemAt(0);
            ui->m_layout_uradora->removeItem(item);
            if(item != ui->m_spacer_ura){
                delete item->widget();
            }
        }
        ui->m_layout_uradora->addItem(ui->m_spacer_ura);
        for(i=0;i<4;i++){
            btn = new OMPaiButton();

            if(i<taku->m_aUradora.size()){
                pai = taku->m_aUradora[i];
            }else{
                pai.m_iCategory = 0;
                pai.m_iNo = 0;
            }

            btn->setPai(pai,0);
            ui->m_layout_uradora->insertWidget(i,btn,0,Qt::AlignLeft | Qt::AlignBottom);
        }
    }


    QMessageBox::information(this,"終局",message);

    m_client.clientStart();
}

void OpenMahjongClient::onDahaiNaki(OMTaku *taku, int memberIndex, OMMember *member, int paiIndex, OMPai pai)
{
    int ind = m_client.getPlayerIndex();
    int cha = OMMember::getChaDistance(ind,memberIndex);

    removeDahai(cha,member->getDahaiRealPos(pai));

}

void OpenMahjongClient::onNakiAdded(OMTaku *taku, int memberIndex, OMMember *member,int mentsuIndex, OMNakiMentsu mentsu,bool kuwae)
{
    OMMentsuWidget *widget = new OMMentsuWidget();
    QLayoutItem *item;
    int ind = m_client.getPlayerIndex();
    int cha = OMMember::getChaDistance(ind,memberIndex);
    int dist = OMMember::getChaDistance(memberIndex,mentsu.m_iAite);
    int i;

    widget->setMentsu(mentsu,cha,dist);


    switch(cha){
    case 0:
        ui->m_layout_tehai0->insertWidget(ui->m_layout_tehai0->count() + 1 - mentsuIndex,widget,0,Qt::AlignRight | Qt::AlignBottom);
        if(!kuwae && member->m_aTehai.size() == 0){
            for(i=0;i<2;i++){
                item = ui->m_layout_tehai0->itemAt(0);
                ui->m_layout_tehai0->removeItem(item);
                item->widget()->deleteLater();
            }
        }
        break;
    case 1:
        ui->m_layout_tehai1->insertWidget(ui->m_layout_tehai1->count() + 1 - mentsuIndex,widget,0,Qt::AlignRight | Qt::AlignTop);
        if(!kuwae && member->m_aTehai.size() == 0){
            for(i=0;i<2;i++){
                item = ui->m_layout_tehai1->itemAt(0);
                ui->m_layout_tehai1->removeItem(item);
                item->widget()->deleteLater();
            }
        }
        break;
    case 2:
        ui->m_layout_tehai2->insertWidget(ui->m_layout_tehai2->count() + 1 - mentsuIndex,widget,0,Qt::AlignLeft | Qt::AlignTop);
        if(!kuwae && member->m_aTehai.size() == 0){
            for(i=0;i<2;i++){
                item = ui->m_layout_tehai2->itemAt(0);
                ui->m_layout_tehai2->removeItem(item);
                item->widget()->deleteLater();
            }
        }
        break;
    case 3:
        ui->m_layout_tehai3->insertWidget(ui->m_layout_tehai3->count() + 1 - mentsuIndex,widget,0,Qt::AlignLeft | Qt::AlignBottom);
        if(!kuwae && member->m_aTehai.size() == 0){
            for(i=0;i<2;i++){
                item = ui->m_layout_tehai3->itemAt(0);
                ui->m_layout_tehai3->removeItem(item);
                item->widget()->deleteLater();
            }
        }
        break;
    default:
        break;
    }


}

void OpenMahjongClient::onNakiRemoved(OMTaku *taku, int memberIndex, OMMember *member,int mentsuIndex, OMNakiMentsu mentsu)
{

}

void OpenMahjongClient::onProgressed(int index, OMTaku *taku)
{
    takuUpdate(taku);
}

void OpenMahjongClient::takuUpdate(OMTaku *taku)
{
    QString mes;

    mes = QString("%1%2局  残り：%3").arg(taku->m_iKyokuCount < 4 ? "東" : "南").arg((taku->m_iKyokuCount % 4)  + 1).arg(taku->m_iYama);
    ui->m_labelState1->setText(mes);

    mes = QString("%1本場  リーチ棒%2").arg(taku->m_iTsumibou).arg(taku->m_iRiichibou);

    ui->m_labelState2->setText(mes);
}

void OpenMahjongClient::onTii()
{
    QSound::play(getFilePath("wav/voice_tii.wav"));
    QSound::play(getFilePath("wav/naki.wav"));
}

void OpenMahjongClient::onDahai()
{
    QSound::play(getFilePath("wav/dahai.wav"));
}

void OpenMahjongClient::onPon()
{
    QSound::play(getFilePath("wav/voice_pon.wav"));
    QSound::play(getFilePath("wav/naki.wav"));
}

void OpenMahjongClient::onRiichi()
{
    QSound::play(getFilePath("wav/voice_riichi.wav"));
    QSound::play(getFilePath("wav/riichi.wav"));
}


void OpenMahjongClient::on_m_btnSyncUpdate_clicked()
{
    int interval = ui->m_edSyncInterval->text().toInt();

    m_client.setSyncInterval(interval);
}
