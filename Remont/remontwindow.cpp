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

//-----------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------
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

    Remont remont = repo.GetCurrentRemontForItem(device.id);
    Q_ASSERT(remont.id > 0);
    remont.startDate = ui->deDate->dateTime();
    repo.UpdateRemont(remont);

    QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 принят в ремонт.")
            .arg(device.type.typeName).arg(device.number).arg(device.type.VNFT));

    ui->lbDevice->clear();
    ui->lbNumber->clear();
    ui->lbVNFT->clear();
    ui->lbClaim->clear();
    ui->lbOrgName->clear();
    ui->lbTypeName->clear();
    device.id = 0;
}


//--------------------------------------------------------------------------------------------
// Поиск по номеру
//--------------------------------------------------------------------------------------------
void RemontWindow::on_tbNumber_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    Items *dev = win->SelectDevice(true, {StatusItem::FAULTY_ON_OSO}, ui->leNumber->text(), true, LoadPartType::ANY_PARENT );
    if(dev != nullptr && dev->id > 0)
    {
        AddDevice(dev);
    }
}

//-----------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------
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
    }
}

//-----------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------
void RemontWindow::AddDevice(Items *dev)
{
    device = *dev;

    QString nameType, iconName;
    dev->GetInfo(nameType, iconName);
    ui->lbNumber->setText(dev->number);
    ui->lbVNFT->setText(dev->type.VNFT);
    ui->lbTypeName->setText(dev->type.typeName);
    ui->lbDevice->setToolTip(nameType);
    ui->lbDevice->setPixmap(QPixmap(iconName));

    claim = repo.GetClaimForItem(dev->id);
    if(claim.id > 0)
    {
        ui->lbOrgName->setText(claim.nameOrganization);
        ui->lbClaim->setText(claim.number + " (" + claim.dateCreate.toString("dd.MM.yyyy") + ")");
    }
    ui->leNumber->clear();

}


