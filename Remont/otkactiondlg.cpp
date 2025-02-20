#include "otkactiondlg.h"
#include "ui_otkactiondlg.h"

OtkActionDlg::OtkActionDlg(Items* item, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OtkActionDlg), dev(item)
{
    ui->setupUi(this);

    QString nameType;
    QString nameIcon;
    item->GetInfo(nameType, nameIcon);

    ui->gbDevice->setTitle(nameType);
    ui->gbDevice->setWindowIcon(QIcon(nameIcon));
    ui->lbNumber->setText(item->number);
    ui->lbVNFT->setText(item->type.VNFT);
    ui->lbTypeName->setText(item->type.typeName);
    ui->lbDateCreate->setText(item->dateCreate.toString("dd.MM.yyyy"));
    ui->lbGarant->setText(QString::number(item->garantMonth));
    ui->lbStatus->setText(item->currStatus);

    ui->wTreeItem->AddItem(dev);

}

OtkActionDlg::~OtkActionDlg()
{
    delete ui;
}

void OtkActionDlg::on_pbOk_clicked()
{
    StatusItem stat = ui->rbAcceptControl->isChecked() ? StatusItem::CORRECT : StatusItem::FAULTY;
    if(dev->AddStatus(*dev, stat, ui->leComment->text()))
        accept();
}

