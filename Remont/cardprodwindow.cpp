#include "cardprodwindow.h"
#include "shipwindow.h"
#include "ui_cardprodwindow.h"
#include <models/claim.h>
#include <models/listproduct.h>
#include <models/remont.h>
#include <models/setterout.h>
#include <models/shipment.h>


//-------------------------------------------------------------------------------------------------------
// Конструктор
//-------------------------------------------------------------------------------------------------------
CardProdWindow::CardProdWindow(Items *device, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CardProdWindow)
{
    SetterOut setter;
    Items root;
    // int idParent = 0;

    ui->setupUi(this);
    setWindowTitle("Карточка \"" + device->GetDefaultName() + "\"");

    QString nameType, iconType;
    device->GetInfo(nameType, iconType);
    device->LoadStatus(*device);

    LoadRemontToForm(device->id);

    loadInclude(device);
    LoadHistoryToForm(device->listStatus);

    root = *device;
    while(root.idParent > 0)
    {
        root = repo.GetItem(root.idParent);
    }

    if(root.idShip > 0)
        ship = repo.GetShipment(root.idShip);
    if(root.idSet > 0)
        setter = repo.GetSetter(root.idSet);
    if(setter.idShip > 0)
        ship = repo.GetShipment(setter.idShip);

    ui->pbToShip->setEnabled(ship.id > 0);

    ui->lbGarant->setText(device->dateGarant.toString("dd.MM.yyyy"));
    ui->lbDateCreate->setText(device->dateCreate.toString("dd.MM.yyyy"));
    ui->lbNumber->setText(device->number);
    ui->lbDateOn->setText(device->dateOn.toString("dd.MM.yyyy"));

    ui->lbGarantMon->setText(QString::number(device->garantMonth));
    ui->lbIcon->setPixmap(QPixmap(iconType));
    ui->lbName->setText(device->name);
    ui->lbNumber2->setText(device->number2);
    ui->lbType->setText(device->type.typeName);
    ui->lbVNFT->setText(device->type.VNFT);

    LoadShipping();
    // if(setter.id > 0)
    // {
    //     ui->lbCardOrder->setText(setter.number);
    // }

    number = device->number;
    // loadShipmentToForm(device);


}

CardProdWindow::~CardProdWindow()
{
    delete ui;
}




//-------------------------------------------------------------------------------------------------------
// Загрузка статусов для выбраннного изделия или модуля
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::LoadHistoryToForm(QList<Status> &listStatus)
{
    ui->twHistory->setRowCount(0);
    int row = 0;
    for(const auto &it : listStatus)
    {
        ui->twHistory->insertRow(row);
        QTableWidgetItem *item = new QTableWidgetItem(it.dateStatus.toString("dd.MM.yyyy"));
        ui->twHistory->setItem(row, 0, item);

        item = new QTableWidgetItem(it.nameStatus);
        ui->twHistory->setItem(row, 1, item);

        item = new QTableWidgetItem(it.Comment);
        ui->twHistory->setItem(row, 2, item);

        ++row;
    }

    ui->twHistory->resizeColumnsToContents();
    ui->twHistory->resizeRowsToContents();
}

//-------------------------------------------------------------------------------------------------------
// Загрузка ремонтов на форму экрана
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::LoadRemontToForm(int idItem)
{
    QList<Remont> listRemont;
    repo.LoadRemont(listRemont, idItem);

    ui->twRemont->setRowCount(listRemont.size());

    int row = 0;
    for(auto &it : listRemont)
    {
        Claim claim = repo.GetClaim(it.idClaim);
        QString reason = repo.GetRemontReason(it.idReason);

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(claim.number);
        ui->twRemont->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setText(it.startDate.toString("dd.MM.yyyy"));
        ui->twRemont->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setText(it.endDate.toString("dd.MM.yyyy"));
        ui->twRemont->setItem(row, 2, item);

        item = new QTableWidgetItem();
        item->setText(reason);
        ui->twRemont->setItem(row, 3, item);

        item = new QTableWidgetItem();
        item->setText(it.action);
        ui->twRemont->setItem(row, 4, item);

        item = new QTableWidgetItem();
        item->setText(it.defect);
        ui->twRemont->setItem(row, 5, item);

        item = new QTableWidgetItem();
        item->setText(it.remark);
        ui->twRemont->setItem(row, 6, item);

        ++row;
    }
}

//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::LoadShipping()
{
    if(ship.id > 0)
    {
        ui->lbDateUPD->setText(ship.dateUPD.toString("dd.MM.yyyy"));
        ui->lbNumberUPD->setText(ship.numberUPD);
        ui->lbSchet->setText(ship.schet);
        ui->lbObjectInstall->setText(ship.objectInstall);
        ui->lbNumberUPD->setText(ship.numberUPD);
        ui->lbDateUPD->setText(ship.dateUPD.toString("dd.MM.yyyy"));
        ui->lbCardOrder->setText(ship.cardOrder);
    }
}


//-------------------------------------------------------------------------------------------------------
// Загрузка набора для выбраннного изделия или модуля
//-------------------------------------------------------------------------------------------------------
// void CardProdWindow::loadShipmentToForm(const Items *prod)
// {
//     SetterOut setter ;//= repo.GetSetter(prod->idSetterOut);
    // Shipment ship = repo.GetShipment(setter.idShipment);

    // if(setter.id <= 0)
    //     return;

    // repo.LoadChildSetter(setter);
    // for(auto &it: setter.listItems)
    // {
    //     repo.LoadChildProduct(it);
    // }

    // ui->lbContract->setText(ship.schet);
    // ui->lbCardOrder->setText(ship.cardOrder);
    // ui->lbObjectInstall->setText(ship.objectInstall);
    // ui->lbProduction->setText(prod->name);
    // ui->lbDateUPD->setText(ship.dateUPD.toString("dd.MM.yyyy"));
    // ui->lbNumberUPD->setText(ship.numberUPD);

    // QTreeWidgetItem *top = new QTreeWidgetItem();
    // top->setText(0, setter.name);
    // top->setIcon(0, QIcon("://image/setter.png"));
    // ui->treeWidget->addTopLevelItem(top);
    // top->setExpanded(true);
    // for(auto const &it : setter.listItems)
    // {
    //     QTreeWidgetItem *child = new QTreeWidgetItem();
    //     child->setIcon(0, QIcon("://image/product.png"));
    //     QString s = it.name + "(" + it.number + ")";
    //     if(it.getIsRepair())
    //         s += " неисправен";
    //     child->setText(0, s);
    //     if(it.number == number)
    //     {
    //         QFont font;
    //         font.setBold(true);
    //         child->setFont(0, font);
    //     }
    //     top->addChild(child);
    //     child->setExpanded(true);
    //     // for(auto mod : it.listModules)
    //     // {
    //     //     // Modul modul = mod;
    //     //     mod.LoadStatus(mod);
    //     //     QTreeWidgetItem *modItem = new QTreeWidgetItem();
    //     //     modItem->setIcon(0, QIcon("://image/modul.png"));
    //     //     s = mod.name + "(" + mod.number + ")";
    //     //     if(mod.getIsRepair())
    //     //         s += " неисправен";
    //     //     modItem->setText(0, s);
    //     //     if(mod.number == number)
    //     //     {
    //     //         QFont font;
    //     //         font.setBold(true);
    //     //         modItem->setFont(0, font);
    //     //     }
    //     //     child->addChild(modItem);
    //     // }
    // }
// }

//-------------------------------------------------------------------------------------------------------
// Загрузка состава изделия
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::loadInclude(const Items *item)
{
    Items dev = *item;
    while(dev.idParent > 0)
        dev = repo.GetItem(dev.idParent);

    ui->treeContent->AddItem(&dev);
    ui->treeContent->SetSelectItem(item->id, item->type.indexType);
}


//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------

void CardProdWindow::on_pbToShip_clicked()
{
    if(ship.id == 0)
        return;

    ShipWindow *win = new ShipWindow(&ship, this);
    if(win->exec() == QDialog::Accepted)
    {
        LoadShipping();
    }

}

