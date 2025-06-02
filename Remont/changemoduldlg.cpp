#include "changemoduldlg.h"
#include "selectdevicewindow.h"
#include "ui_changemoduldlg.h"

#include <QMessageBox>

#include <models/claim.h>

ChangeModulDlg::ChangeModulDlg(Items* dev, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeModulDlg), brokenDev(dev)
{
    ui->setupUi(this);

    QString nameType, nameIcon;
    dev->GetInfo(nameType, nameIcon);

    ui->lbDevice->setPixmap(QPixmap(nameIcon));
    ui->lbNumber->setText(dev->number);
    ui->lbType->setText(dev->type.typeName);
    ui->lbVNFT->setText(dev->type.VNFT);

    Items parentDev = repo.GetItem(dev->idParent);
    ui->wTree->AddItem(&parentDev);
    ui->wTree->SetSelectItem(dev->id, dev->type.indexType);
}

ChangeModulDlg::~ChangeModulDlg()
{
    delete ui;
}

//---------------------------------------------------------------------------
// поиск обрудования
//---------------------------------------------------------------------------
void ChangeModulDlg::on_tbSearch_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(brokenDev->type.indexType);
    Items *dev =  win->SelectDevice(true, {StatusItem::CORRECT, StatusItem::CORRECT_OSO}, ui->leSwarch->text());
    if(dev != nullptr && dev->id > 0)
    {
        newDev = *dev;
        ui->lbNumber_2->setText(dev->number);
        ui->lbType_2->setText(dev->type.typeName);
        ui->lbVNFT_2->setText(dev->type.VNFT);
    }
}


//---------------------------------------------------------------------------
// Кнопка ОК
//---------------------------------------------------------------------------
void ChangeModulDlg::on_pbOK_clicked()
{
    QString comment = "на № %1";
    brokenDev->AddStatus(*brokenDev, StatusItem::EXCHANGE, comment.arg(newDev.number), brokenDev->idParent);
    newDev.AddStatus(newDev, StatusItem::INSTALL, QString("замена № %1").arg(brokenDev->number));
    newDev.AddStatus(newDev, StatusItem::WORK);
    newDev.idParent = brokenDev->idParent;
    repo.UpdateItem(newDev);

    Items parent = repo.GetItem(brokenDev->idParent);
    do
    {
        parent.LoadStatus(parent);
        bool res = parent.TestChildGoodStatus();
        if(res && parent.listStatus.size() > 0 && parent.listStatus.last().idStatus == StatusItem::FAULTY_CHILD)
            parent.AddStatus(parent, StatusItem::WORK);

        parent = repo.GetItem(parent.idParent);
    } while(parent.idParent > 0);

    Claim claim = repo.GetClaimForItem(brokenDev->id);

    brokenDev->idParent = 0;
    repo.UpdateItem(*brokenDev);

    claim.CheckAndClose();

    QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 земенен.")
            .arg(brokenDev->type.typeName).arg(brokenDev->number).arg(brokenDev->type.VNFT));

    accept();
}

