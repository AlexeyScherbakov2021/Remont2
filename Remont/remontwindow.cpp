#include "claimwindow.h"
#include "remontwindow.h"
#include "selectdevicewindow.h"
#include "ui_remontwindow.h"

RemontWindow::RemontWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemontWindow)
{
    ui->setupUi(this);

    ui->deDate->setDateTime(QDateTime::currentDateTime());

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
    // Status status;
    // status.idStatus = Status::Stat::REMONT;
    // status.dateStatus = ui->deDate->dateTime();

    if(idProd != 0)
    {
        Product prod;
        prod.id = idProd;
        // status.idDevice = idProd;
        prod.AddStatus(prod, Status::REMONT);
    }

    if(idMod != 0)
    {
        Modul mod;
        mod.id = idMod;
        // status.idDevice = idMod;
        mod.AddStatus(mod, Status::REMONT);
    }
    idProd = idMod = 0;
}


void RemontWindow::on_tbNumber_clicked()
{
    idMod = idProd = 0;

    // SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumber->text(),Status::FAULTY_ON_OSO);
    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    IDevice *dev = win->SelectDevice(true, ui->leNumber->text(),Status::FAULTY_ON_OSO);
    if(dev != nullptr)
    // if(win->exec() == QDialog::Accepted)
    {
        Claim claim;
        if(dev->typeDevice == ev::MODUL)
        {
            Modul* modul = static_cast<Modul*>(dev);
            if(repo.LoadClaimForModul(modul->id, claim))
                ui->lbClaim->setText("№" + claim.number + " от " + claim.dateRegister.toString("dd.MM.yyyy"));

            ui->lbNumber->setText(modul->number);
            ui->lbName->setText(modul->name);
            ui->lbDevice->setText("Модуль");
            idMod = modul->id;
        }

        if(dev->typeDevice == ev::PRODUCT)
        {
            Product* prod = static_cast<Product*>(dev);
            if(repo.LoadClaimForProduct(prod->id, claim))
                ui->lbClaim->setText("№" + claim.number + " от " + claim.dateRegister.toString("dd.MM.yyyy"));

            ui->lbNumber->setText(prod->number);
            ui->lbName->setText(prod->name);
            ui->lbDevice->setText("Изделие");
            idProd = prod->id;
        }
        ui->leNumber->clear();
    }

}

