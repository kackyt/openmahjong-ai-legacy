#include "AgariDialog.h"
#include "ui_AgariDialog.h"

AgariDialog::AgariDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgariDialog)
{
    ui->setupUi(this);
}

AgariDialog::~AgariDialog()
{
    delete ui;
}
