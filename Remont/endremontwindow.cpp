#include "endremontwindow.h"
#include "selectdevicewindow.h"
#include "ui_endremontwindow.h"

#include <models/claim.h>

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



void EndRemontWindow::on_tbNumber_clicked()
{
    idMod =idProd = 0;

    SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumber->text(),Status::REMONT);
    if(win->exec() == QDialog::Accepted)
    {
        Claim claim;
        if(win->modul.id != 0)
        {
            if(repo.LoadClaimForModul(win->modul.id, claim))
                ui->lbClaim->setText("№" + claim.number + " от " + claim.dateRegister.toString("dd.MM.yyyy"));

            ui->lbNumber->setText(win->modul.number);
            ui->lbName->setText(win->modul.name);
            ui->lbDevice->setText("Модуль");
            idMod = win->modul.id;
        }

        if(win->prod.id != 0)
        {
            if(repo.LoadClaimForProduct(win->prod.id, claim))
                ui->lbClaim->setText("№" + claim.number + " от " + claim.dateRegister.toString("dd.MM.yyyy"));

            ui->lbNumber->setText(win->prod.number);
            ui->lbName->setText(win->prod.name);
            ui->lbDevice->setText("Изделие");
            idProd = win->prod.id;
        }
        ui->leNumber->clear();
    }

}


void EndRemontWindow::on_pbEndRemont_clicked()
{
    Status::Stat stat = ui->cbScrap->isChecked() ? Status::END_WORK : Status::CORRECT_OSO;

    if(idProd != 0)
    {
        Product prod;
        prod.id = idProd;
        prod.AddStatus(prod, stat, ui->deDate->dateTime());
    }

    if(idMod != 0)
    {
        Modul mod;
        mod.id = idMod;
        mod.AddStatus(mod, stat, ui->deDate->dateTime());
    }

    idProd = idMod = 0;

}

