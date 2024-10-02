#include "acceptremontwindow.h"
#include "complectproductwindow.h"
#include "selectdevicewindow.h"
#include "ui_acceptremontwindow.h"
#include <models/product.h>


#include <QMessageBox>

AcceptRemontWindow::AcceptRemontWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AcceptRemontWindow)
{
    ui->setupUi(this);

    claims.Load();

    auto newEnd = std::remove_if(claims.listItems.begin(), claims.listItems.end(), [](Claim claim) { return !claim.DateOut.isNull();});

    claims.listItems.erase(newEnd, claims.listItems.end());

    for(auto &it : claims.listItems)
    {
        repo.LoadClaimProducts(it.id, it.listProduct);
        repo.LoadClaimModules(it.id, it.listModul);
        // ui->cbClaim->addItem(it.Number, it.id);
    }

    ui->deDate->setDateTime(QDateTime::currentDateTime());

    // QMap<int, QString> listReason;
    // repo.LoadRemontReason(listReason);

    // for(auto it = listReason.cbegin(); it != listReason.cend(); ++it)
    // {
    //     ui->cbReason->addItem(*it, it.key());
    // }

}

AcceptRemontWindow::~AcceptRemontWindow()
{
    delete ui;
}

void AcceptRemontWindow::on_pbExchange_clicked()
{
    ComplectProductWindow *win = new ComplectProductWindow(this);
    win->show();
}


//------------------------------------------------------------------------
// Кнопка принят в ремонт
//------------------------------------------------------------------------
void AcceptRemontWindow::on_pbApply_clicked()
{

    // TypeDevice type = (TypeDevice)ui->cbDevice->currentData(Qt::UserRole + 1).toInt();
    // int id = ui->cbDevice->currentData(Qt::UserRole).toInt();

    // Status status;
    // status.idStatus = Status::Stat::FAULTY_ON_OSO;
    // status.dateStatus = ui->deDate->dateTime();

    if(idProd != 0)
    {
        Product prod;
        prod.id = idProd;
        // status.idDevice = idProd;
        prod.AddStatus(prod, Status::FAULTY_ON_OSO);
    }

    if(idMod != 0)
    {
        Modul mod;
        mod.id = idMod;
        // status.idDevice = idMod;
        mod.AddStatus(mod, Status::FAULTY_ON_OSO);;
    }

    idProd = idMod = 0;

    // RemontM remont;
    // remont.workDate = QDateTime::currentDateTime();
    // remont.reclamtionId = ui->cbClaim->currentData().toInt();
    // remont.reasonId = ui->cbReason->currentData().toInt();
    // remont.action = ui->leAction->text();
    // remont.defect = ui->leDefect->text();

    // if(type == TypeDevice::Product)
    // {
    //     repo.AddStatusProduct(status);
    //     // repo.AddRemontP(remont);
    // }
    // else
    // {
    //     repo.AddStatusModul(status);
    //     // repo.AddRemontM(remont);
    // }

}


void AcceptRemontWindow::on_tbNumber_clicked()
{
    idMod =idProd = 0;

    SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumber->text(),Status::FAULTY_ON_OBJECT);
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

