#include "endremontwindow.h"
#include "selectdevicewindow.h"
#include "ui_endremontwindow.h"
#include <models/claim.h>
#include <models/remont.h>
#include <QMessageBox>

EndRemontWindow::EndRemontWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EndRemontWindow)
{
    ui->setupUi(this);

    ui->deDate->setDateTime(QDateTime::currentDateTime());

    QMap<int, QString> listReason;
    repo.LoadRemontReason(listReason);

    for( auto it = listReason.begin(); it != listReason.end(); ++it)
    {
        ui->cbReason->addItem(*it, it.key());
    }

    ui->cbReason->setCurrentIndex(0);

}

EndRemontWindow::~EndRemontWindow()
{
    delete ui;
}


//---------------------------------------------------------------------------------------
// Кнопка Поиск по номеру
//---------------------------------------------------------------------------------------
void EndRemontWindow::on_tbNumber_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    Items *dev = win->SelectDevice(true, {StatusItem::REMONT}, ui->leNumber->text(), true, true );
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


//---------------------------------------------------------------------------------------
// Кнопка Закончить ремонт
//---------------------------------------------------------------------------------------
void EndRemontWindow::on_pbEndRemont_clicked()
{
    StatusItem stat = ui->cbScrap->isChecked() ? StatusItem::END_WORK : StatusItem::CORRECT_OSO;

    if(device.id == 0)
        return;


    // закрыть рекламацию, если все отремонтировано
    // claim.isClosed = true;
    // repo.UpdateItem(claim);

    // внести изменения в таблицы ремонтов

    Remont rem = repo.GetRemontForItem(device.id);
    Q_ASSERT(rem.id != 0);
    Q_ASSERT(ui->cbReason->currentData(Qt::UserRole) != QVariant());

    rem.action = ui->leAction->text();
    rem.defect = ui->leDefect->text();
    rem.endDate = ui->deDate->dateTime();
    rem.idReason = ui->cbReason->currentData(Qt::UserRole).toInt();
    rem.remark = ui->ptRemark->document()->toPlainText();

    repo.UpdateRemont(rem);

    device.AddStatus(device, stat, ui->deDate->dateTime());

    QMessageBox::information(this, "Сообщение", QString("%1 №%2 %3 ремонт завершен.")
                .arg(device.type.typeName).arg(device.number).arg(device.type.VNFT));

    ui->lbDevice->clear();
    ui->lbNumber->clear();
    ui->lbVNFT->clear();
    ui->lbClaim->clear();
    ui->lbOrgName->clear();
    ui->lbTypeName->clear();
    device.id = 0;


    // if(product.id != 0)
    // {
    //     // Product prod;
    //     // prod.id = idProd;
    //     product.AddStatus(product, stat, ui->deDate->dateTime());
    //     Remont rem = repo.GetCurrentRemont(product.id, ev::PRODUCT);
    //     rem.action = ui->leAction->text();
    //     rem.defect = ui->leDefect->text();
    //     rem.endDate = ui->deDate->dateTime();
    //     rem.remark = ui->ptRemark->document()->toPlainText();
    //     rem.idReason = ui->cbReason->currentData(Qt::UserRole).toInt();
    //     repo.UpdateRemont(rem, ev::PRODUCT);
    // }

    // if(modul.id != 0)
    // {
    //     // Modul mod;
    //     // mod.id = idMod;
    //     modul.AddStatus(modul, stat, ui->deDate->dateTime());
    //     Remont rem = repo.GetCurrentRemont(modul.id, ev::MODUL);
    //     rem.action = ui->leAction->text();
    //     rem.defect = ui->leDefect->text();
    //     rem.endDate = ui->deDate->dateTime();
    //     rem.remark = ui->ptRemark->document()->toPlainText();
    //     rem.idReason = ui->cbReason->currentData(Qt::UserRole).toInt();
    //     repo.UpdateRemont(rem, ev::MODUL);

    //     if(modul.idParent != 0)
    //     {
    //         // product = repo.GetProduct(modul.idParent);
    //         product.AddStatus(product, StatusItem::WORK , ui->deDate->dateTime());
    //     }

    // }

    // idProd = idMod = 0;

    // QMessageBox::information(this, "Сообщение", QString("Для %1 #%2 %3 ремонт завершен.")
    //             .arg(ui->lbDevice->text()).arg(ui->lbNumber->text()).arg(ui->lbName->text()));

    // ui->lbNumber->clear();
    // ui->lbName->clear();
    // ui->lbDevice->clear();
    // ui->leNumber->clear();
    // ui->ptRemark->clear();
    // ui->lbClaim->clear();

}

