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
    // product.id = 0;
    // modul.id = 0;
    // // idMod =idProd = 0;

    // // SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumber->text(),Status::REMONT);
    // // if(win->exec() == QDialog::Accepted)
    // SelectDeviceWindow *win = new SelectDeviceWindow(this);
    // Items *dev = win->SelectDevice(true, ui->leNumber->text(),Status::REMONT);
    // if(dev != nullptr)
    // {
    //     Claim claim;
    //     // if(dev->typeDevice == ev::MODUL)
    //     if(dev->type.indexType == ItemType::Modul)
    //     {
    //         // Modul* mod = static_cast<Modul*>(dev);
    //         modul = *(static_cast<Items*>(dev));
    //         // if(repo.LoadClaimForModul(modul.id, claim))
    //         //     ui->lbClaim->setText("№" + claim.number + " от " + claim.dateRegister.toString("dd.MM.yyyy"));

    //         ui->lbNumber->setText(modul.number);
    //         ui->lbName->setText(modul.name);
    //         ui->lbDevice->setText("Модуль");
    //         // idMod = modul->id;
    //     }

    //     // if(dev->type.IndexType == ev::PRODUCT)
    //     // if(dev->type.IndexType == ItemType::Product)
    //     if(dev->type.indexType == ItemType::Product)
    //     {
    //         // Product* prod = static_cast<Product*>(dev);
    //         product = *(static_cast<Items*>(dev));
    //         // if(repo.LoadClaimForProduct(product.id, claim))
    //         //     ui->lbClaim->setText("№" + claim.number + " от " + claim.dateRegister.toString("dd.MM.yyyy"));

    //         ui->lbNumber->setText(product.number);
    //         ui->lbName->setText(product.name);
    //         ui->lbDevice->setText("Изделие");
    //         // idProd = prod->id;
    //     }
    //     ui->leNumber->clear();
    // }

}


//---------------------------------------------------------------------------------------
// Кнопка Закончить ремонт
//---------------------------------------------------------------------------------------
void EndRemontWindow::on_pbEndRemont_clicked()
{
    StatusItem stat = ui->cbScrap->isChecked() ? StatusItem::END_WORK : StatusItem::CORRECT_OSO;

    if(product.id != 0)
    {
        // Product prod;
        // prod.id = idProd;
        product.AddStatus(product, stat, ui->deDate->dateTime());
        Remont rem = repo.GetCurrentRemont(product.id, ev::PRODUCT);
        rem.action = ui->leAction->text();
        rem.defect = ui->leDefect->text();
        rem.endDate = ui->deDate->dateTime();
        rem.remark = ui->ptRemark->document()->toPlainText();
        rem.idReason = ui->cbReason->currentData(Qt::UserRole).toInt();
        repo.UpdateRemont(rem, ev::PRODUCT);
    }

    if(modul.id != 0)
    {
        // Modul mod;
        // mod.id = idMod;
        modul.AddStatus(modul, stat, ui->deDate->dateTime());
        Remont rem = repo.GetCurrentRemont(modul.id, ev::MODUL);
        rem.action = ui->leAction->text();
        rem.defect = ui->leDefect->text();
        rem.endDate = ui->deDate->dateTime();
        rem.remark = ui->ptRemark->document()->toPlainText();
        rem.idReason = ui->cbReason->currentData(Qt::UserRole).toInt();
        repo.UpdateRemont(rem, ev::MODUL);

        if(modul.idParent != 0)
        {
            // product = repo.GetProduct(modul.idParent);
            product.AddStatus(product, StatusItem::WORK , ui->deDate->dateTime());
        }

    }

    product.id = 0;
    modul.id = 0;
    // idProd = idMod = 0;

    QMessageBox::information(this, "Сообщение", QString("Для %1 #%2 %3 ремонт завершен.")
                .arg(ui->lbDevice->text()).arg(ui->lbNumber->text()).arg(ui->lbName->text()));

    ui->lbNumber->clear();
    ui->lbName->clear();
    ui->lbDevice->clear();
    ui->leNumber->clear();
    ui->ptRemark->clear();
    ui->lbClaim->clear();

}

