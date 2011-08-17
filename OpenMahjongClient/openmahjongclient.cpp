#include "openmahjongclient.h"
#include "ui_openmahjongclient.h"
#include "ConnectDialog.h"

OpenMahjongClient::OpenMahjongClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenMahjongClient)
{
    ui->setupUi(this);
}

OpenMahjongClient::~OpenMahjongClient()
{
    delete ui;
}

void OpenMahjongClient::on_m_btnConnect_clicked()
{
    ConnectDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted){
        /* ƒpƒ‰ƒ[ƒ^‚ğİ’è‚µ‚ÄÚ‘± */
        QVector<QString> playernames,compnames;
        QUrl url;
        OM_CONNECTION_TYPE conType;
        dialog.getPlayerName(playernames);
        dialog.getCompName(compnames);
        m_client.setPlayerName(playernames,compnames);
        dialog.getUrl(url);
        m_client.setDestination(url);
        conType = dialog.getConnectionType();
        ((OMGenericClient*)&m_client)->connect(conType,dialog.getSessionNum());
    }
}

void OpenMahjongClient::appendMessageText(QString &)
{

}

void OpenMahjongClient::enableCommand(OMCommand &)
{

}
