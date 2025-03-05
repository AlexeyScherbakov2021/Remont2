#include "claimwindow.h"
#include "remontwindow.h"
#include "scan.h"
#include "selectdevicewindow.h"
#include "ui_remontwindow.h"
#include <QMessageBox>
#include <models/remont.h>

RemontWindow::RemontWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemontWindow)
{
    ui->setupUi(this);

    ui->deDate->setDateTime(QDateTime::currentDateTime());
    connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

RemontWindow::~RemontWindow()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------
// Кнопка OK
//--------------------------------------------------------------------------------------------
void RemontWindow::on_pbOK_clicked()
{

    if(device.id == 0)
        return;

    device.AddStatus(device, StatusItem::REMONT, ui->deDate->dateTime());

    QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 принят в ремонт.")
                                                    .arg(device.type.typeName).arg(device.number).arg(device.type.VNFT));

    ui->lbDevice->clear();
    ui->lbNumber->clear();
    ui->lbVNFT->clear();
    ui->lbClaim->clear();
    ui->lbOrgName->clear();
    ui->lbTypeName->clear();
    device.id = 0;

    // Remont remont;
    // remont.startDate = ui->deDate->dateTime();
    // remont.idReclamation = claim.id;

    // if(idProd != 0)
    // {
    //     Items prod;
    //     prod.id = idProd;
    //     prod.AddStatus(prod, StatusItem::REMONT);
    //     remont.idParent = idProd;
    //     repo.AddRemont(remont, ev::PRODUCT);
    // }

    // if(idMod != 0)
    // {
    //     Items mod;
    //     mod.id = idMod;
    //     mod.AddStatus(mod, StatusItem::REMONT);
    //     remont.idParent = idMod;
    //     repo.AddRemont(remont, ev::MODUL);
    // }

    // QMessageBox::information(this, "Сообщение", QString("%1 #%2 %3 принят в ремонт.")
    //             .arg(ui->lbDevice->text()).arg(ui->lbNumber->text()).arg(ui->lbTypeName->text()));

    // ui->lbDevice->clear();
    // ui->lbNumber->clear();
    // ui->lbTypeName->clear();
    // ui->lbClaim->clear();

}


//--------------------------------------------------------------------------------------------
// Поиск по номеру
//--------------------------------------------------------------------------------------------
void RemontWindow::on_tbNumber_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    Items *dev = win->SelectDevice(true, {StatusItem::FAULTY_ON_OSO}, ui->leNumber->text(), true, true );
    if(dev != nullptr && dev->id > 0)
    {
        AddDevice(dev);
        // device = *dev;
        // Claim claim = repo.GetClaimForItem(dev->id);

        // QString nameType, iconName;
        // dev->GetInfo(nameType, iconName);
        // ui->lbNumber->setText(dev->number);
        // ui->lbVNFT->setText(dev->type.VNFT);
        // ui->lbTypeName->setText(dev->type.typeName);
        // ui->lbDevice->setToolTip(nameType);
        // ui->lbDevice->setPixmap(QPixmap(iconName));
        // ui->lbOrgName->setText(claim.nameOrganization);
        // ui->lbClaim->setText(claim.number + " (" + claim.dateCreate.toString("dd.MM.yyyy") + ")");
        // ui->leNumber->clear();
    }
}

void RemontWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        ui->leNumber->setText(s);
        Items item = repo.GetItem2(s, {StatusItem::FAULTY_ON_OSO}, true, true);
        if(item.id > 0)
        {
            AddDevice(&item);
        }
        // on_tbNumber_clicked();
    }
}


void RemontWindow::AddDevice(Items *dev)
{
    device = *dev;
    claim = repo.GetClaimForItem(dev->id);

    QString nameType, iconName;
    dev->GetInfo(nameType, iconName);
    ui->lbNumber->setText(dev->number);
    ui->lbVNFT->setText(dev->type.VNFT);
    ui->lbTypeName->setText(dev->type.typeName);
    ui->lbDevice->setToolTip(nameType);
    ui->lbDevice->setPixmap(QPixmap(iconName));
    ui->lbOrgName->setText(claim.nameOrganization);
    ui->lbClaim->setText(claim.number + " (" + claim.dateCreate.toString("dd.MM.yyyy") + ")");
    ui->leNumber->clear();

}


