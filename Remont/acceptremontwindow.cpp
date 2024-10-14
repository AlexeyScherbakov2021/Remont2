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


//-------------------------------------------------------------------------------------------------
// Кнопка Произвести замену
//-------------------------------------------------------------------------------------------------
void AcceptRemontWindow::on_pbExchange_clicked()
{
    if(idMod == 0 || idParentProd == 0)
    {
        QMessageBox::critical(this, "Предупреждение", "Заменить можно только модуль в составе изделия.");
        return;
    }

    Product prod = repo.GetProduct(idParentProd);
    ComplectProductWindow *win = new ComplectProductWindow(this, &prod);
    win->exec();
}


//------------------------------------------------------------------------
// Кнопка принять в ремонт
//------------------------------------------------------------------------
void AcceptRemontWindow::on_pbApply_clicked()
{
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

    QMessageBox::information(this, "Сообщение", QString("%1 #%2 %3 принят в ОСО.")
                .arg(ui->lbDevice->text()).arg(ui->lbNumber->text()).arg(ui->lbName->text()));

    ui->lbDevice->clear();
    ui->lbNumber->clear();
    ui->lbName->clear();
    ui->lbClaim->clear();

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


//-------------------------------------------------------------------------------------------------
// Кнопка Найти устройство
//-------------------------------------------------------------------------------------------------
void AcceptRemontWindow::on_tbNumber_clicked()
{
    idMod =idProd = 0;

    // SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumber->text(),Status::FAULTY_ON_OBJECT);
    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    IDevice *dev = win->SelectDevice(true, ui->leNumber->text(), Status::FAULTY_ON_OBJECT);

    if(dev != nullptr)
    {
        Claim claim;
        // modul = dynamic_cast<Modul*> (dev);
        if(dev->typeDevice == ev::MODUL)
        // if(win->modul->id != 0)
        {
            Modul *modul = static_cast<Modul*> (dev);
            if(repo.LoadClaimForModul(modul->id, claim))
                ui->lbClaim->setText("№" + claim.number + " от " + claim.dateRegister.toString("dd.MM.yyyy"));

            ui->lbNumber->setText(modul->number);
            ui->lbName->setText(modul->name);
            ui->lbDevice->setText("Модуль");
            idMod = modul->id;
            idParentProd = modul->idProduct;
        }

        // prod = dynamic_cast<Product*> (dev);
        if(dev->typeDevice == ev::PRODUCT )
        {
            Product *prod = static_cast<Product*> (dev);
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

