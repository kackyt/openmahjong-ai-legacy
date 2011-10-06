#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

OM_CONNECTION_TYPE ConnectDialog::getConnectionType() const
{
    OM_CONNECTION_TYPE conType = OM_CONNECTION_TYPE_CREATE;
    if(ui->rdCreate->isChecked()){
        conType = OM_CONNECTION_TYPE_CREATE;
    }else if(ui->rdConnect->isChecked()){
        conType = OM_CONNECTION_TYPE_CONNECT;
    }else if(ui->rdReconnect->isChecked()){
        conType = OM_CONNECTION_TYPE_RECONNECT;
    }else if(ui->rdDebug->isChecked()){
        conType = OM_CONNECTION_TYPE_DEBUG;
    }

    return conType;
}

void ConnectDialog::getPlayerName(OMArray<OMString> &playernames) const
{
    if(ui->edPlayerName->text() != ""){
        playernames.clear();
        playernames.append(ui->edPlayerName->text());
    }
}

void ConnectDialog::getCompName(OMArray<OMString> &compnames) const
{
    compnames.clear();

    if(ui->cmbComp1->currentText() != ""){
        compnames.append(ui->cmbComp1->currentText());
    }

    if(ui->cmbComp2->currentText() != ""){
        compnames.append(ui->cmbComp2->currentText());
    }

    if(ui->cmbComp3->currentText() != ""){
        compnames.append(ui->cmbComp3->currentText());
    }
}

int ConnectDialog::getSessionNum() const
{
    bool ok;
    int num = 0;

    num = (int)ui->edTakuNum->text().toUInt(&ok);

    if(!ok){
        num = 0;
    }

    return num;
}

void ConnectDialog::getUrl(QUrl &url) const
{
    url = QUrl(ui->cmbUrl->currentText());
}
