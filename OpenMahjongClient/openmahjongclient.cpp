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
    if(dialog.exec()){
        /* ê⁄ë± */
    }
}

void OpenMahjongClient::appendMessageText(QString &)
{

}

void OpenMahjongClient::enableCommand(OMCommand &)
{

}
