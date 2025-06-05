#include "statusdlg.h"
#include "ui_statusdlg.h"


StatusDlg::StatusDlg(Status &stat, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StatusDlg), status(&stat)
{
    ui->setupUi(this);

    ui->deDate->setDateTime(QDateTime::currentDateTime());

    RepoMSSQL repo;
    repo.loadAllSatus(listStatus);
    for(auto it = listStatus.cbegin(); it != listStatus.cend(); ++it)
        ui->cbStatus->addItem(it.key());

}

StatusDlg::~StatusDlg()
{
    delete ui;
}

void StatusDlg::on_pbOK_clicked()
{
    status->dateStatus = ui->deDate->dateTime();
    status->idStatus = StatusItem::CREATE;
    status->numberDoc = ui->leNumDoc->text();
    status->Comment = ui->leComment->text();
    status->nameStatus = ui->cbStatus->currentText();
    status->idStatus = (StatusItem)listStatus[status->nameStatus];
    accept();
}

