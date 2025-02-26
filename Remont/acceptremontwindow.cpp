#include "acceptremontwindow.h"
#include "complectproductwindow.h"
#include "selectdevicewindow.h"
#include "ui_acceptremontwindow.h"
// #include <models/product.h>


#include <QMessageBox>

#include <models/remont.h>

AcceptRemontWindow::AcceptRemontWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AcceptRemontWindow)
{
    ui->setupUi(this);
    ui->deDate->setDateTime(QDateTime::currentDateTime());

}

AcceptRemontWindow::~AcceptRemontWindow()
{
    delete ui;
}


//-------------------------------------------------------------------------------------------------
// Кнопка Произвести замену
//-------------------------------------------------------------------------------------------------
void AcceptRemontWindow::on_pbExchange_clicked()
{
    if(device.idParent == 0)
    {
        QMessageBox::critical(this, "Предупреждение", "Заменить можно только модуль в составе изделия.");
        return;
    }

    Items prod = repo.GetItem(device.idParent);
    ComplectProductWindow *win = new ComplectProductWindow(this, &prod);
    win->exec();
}


//------------------------------------------------------------------------
// Кнопка принять в ремонт
//------------------------------------------------------------------------
void AcceptRemontWindow::on_pbApply_clicked()
{
    if(device.id == 0)
        return;

    device.AddStatus(device, StatusItem::FAULTY_ON_OSO, ui->deDate->dateTime());

    QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 принят в ОСО.")
                .arg(device.type.typeName).arg(device.number).arg(device.type.VNFT));

    // Добавление в ремонт

    Remont remont;
    remont.idClaim = claim.id;
    remont.idItem = device.id;
    remont.startDate = ui->deDate->dateTime();
    repo.AddRemont(remont);

    ui->lbDevice->clear();
    ui->lbNumber->clear();
    ui->lbVNFT->clear();
    ui->lbClaim->clear();
    ui->lbOrgName->clear();
    ui->lbTypeName->clear();
    device.id = 0;

}


//-------------------------------------------------------------------------------------------------
// Кнопка Найти устройство
//-------------------------------------------------------------------------------------------------
void AcceptRemontWindow::on_tbNumber_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    Items *dev = win->SelectDevice(true, {StatusItem::FAULTY_ON_OBJECT}, ui->leNumber->text(), true, true );
    if(dev != nullptr && dev->id > 0)
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
}

