#include "claimwindow.h"
#include "remontwindow.h"
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
    Remont remont;
    remont.startDate = ui->deDate->dateTime();
    remont.idReclamation = claim.id;

    if(idProd != 0)
    {
        Product prod;
        prod.id = idProd;
        prod.AddStatus(prod, Status::REMONT);
        remont.idParent = idProd;
        repo.AddRemont(remont, ev::PRODUCT);
    }

    if(idMod != 0)
    {
        Modul mod;
        mod.id = idMod;
        mod.AddStatus(mod, Status::REMONT);
        remont.idParent = idMod;
        repo.AddRemont(remont, ev::MODUL);
    }

    QMessageBox::information(this, "Сообщение", QString("%1 #%2 %3 принят в ремонт.")
                .arg(ui->lbDevice->text()).arg(ui->lbNumber->text()).arg(ui->lbName->text()));

    ui->lbDevice->clear();
    ui->lbNumber->clear();
    ui->lbName->clear();
    ui->lbClaim->clear();

}


//--------------------------------------------------------------------------------------------
// Поиск по номеру
//--------------------------------------------------------------------------------------------
void RemontWindow::on_tbNumber_clicked()
{
    idMod = idProd = 0;

    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    IDevice *dev = win->SelectDevice(true, ui->leNumber->text(),Status::FAULTY_ON_OSO);
    if(dev != nullptr)
    {
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

