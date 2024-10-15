#include "cardprodwindow.h"
#include "ui_cardprodwindow.h"
#include <models/claim.h>
#include <models/listproduct.h>
#include <models/remont.h>
#include <models/setterout.h>
#include <models/shipment.h>


//-------------------------------------------------------------------------------------------------------
// Конструктор
//-------------------------------------------------------------------------------------------------------
CardProdWindow::CardProdWindow(const QString &name, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CardProdWindow)
{
    ui->setupUi(this);
    setWindowTitle("Карточка изделия " + name);

}

//-------------------------------------------------------------------------------------------------------
// Конструктор для модуля
//-------------------------------------------------------------------------------------------------------
// CardProdWindow::CardProdWindow(Modul &mod, QWidget *parent) : CardProdWindow(mod.name, parent)
// {
//     // modul = mod;

//     // remontEntity = new RemontModul(mod);

//     // qDebug() << &mod << &((RemontModul*)remontEntity)->modul;
//     // qDebug() << &mod.listRemont << remontEntity->listRemont;

//     Product prod = repo.GetProduct(mod.idProduct);
//     number = mod.number;
//     // ui->leNumber->setText(mod.number);
//     loadShipmentToForm(&prod);

//     // remontEntity->LoadListRemont(&repo);
//     // repo.LoadRemontM(modul.listRemont, modul.id);

//     LoadHistoryToForm(mod.listStatus);

//     // for(auto &it : *remontEntity->listRemont)
//     //     AddRowRemont(it);

//     // for(auto &it : modul.listRemont)
//     // {
//     //     AddRowRemont(it);
//     // }

// }


//-------------------------------------------------------------------------------------------------------
// Конструктор для изделия
//-------------------------------------------------------------------------------------------------------
// CardProdWindow::CardProdWindow(Product &prod, QWidget *parent) : CardProdWindow(prod.name, parent)
// {
//     // remontEntity = new RemontProduct(prod);

//     // // qDebug() << &prod << &((RemontProduct*)remontEntity)->product;
//     // // qDebug() << &prod.listRemont << remontEntity->listRemont;

//     // ListProduct lProduct;
//     // lProduct.LoadChild(prod);

//     repo.LoadChildProduct(prod);
//     prod.LoadStatus(prod);

//     // repo.LoadChildProduct(prod);
//     number = prod.number;
//     loadInclude(&prod);
//     loadShipmentToForm(&prod);

//     LoadHistoryToForm(prod.listStatus);

//     // remontEntity->LoadListRemont(&repo);
//     // for(auto &it : *remontEntity->listRemont)
//     //     AddRowRemont(it);

// }

CardProdWindow::CardProdWindow(IDevice *device, QWidget *parent) : CardProdWindow(device->name, parent)
{
    Product *prod;
    Product product;
    QList<Remont> listRemont;

    if(device->typeDevice == ev::MODUL)
    {
        Modul *mod = static_cast<Modul*>(device);
        product = repo.GetProduct(mod->idProduct);
        prod = &product;
        mod->LoadStatus(*mod);
        LoadHistoryToForm(mod->listStatus);
        repo.LoadRemont(listRemont, mod->id, ev::MODUL);
    }
    if(device->typeDevice == ev::PRODUCT)
    {
        prod = static_cast<Product*>(device);
        repo.LoadChildProduct(*prod);
        prod->LoadStatus(*prod);
        loadInclude(prod);
        LoadHistoryToForm(prod->listStatus);
        repo.LoadRemont(listRemont, prod->id, ev::PRODUCT);
    }
    LoadRemontToForm(listRemont);

    ui->lbGarant->setText(device->EndGarant.toString("dd.MM.yyyy"));
    ui->lbDateCreate->setText(device->dateRegister.toString("dd.MM.yyyy"));
    ui->lbNumber->setText(device->number);
    ui->lbDateOn->setText(device->dateOn.toString("dd.MM.yyyy"));
    // qDebug() << device->dateRegister << device->EndGarant;

    number = device->number;
    loadShipmentToForm(prod);

}

CardProdWindow::~CardProdWindow()
{
    delete ui;
}

// void CardProdWindow::AddRowRemont(RemontM &rem)
// {
//     QTreeWidgetItem *top = new QTreeWidgetItem();
//     // top->setText(0, rem.reclamtion);
//     top->setText(1, rem.workDate.toString("dd.MM.yyyy"));
//     top->setText(2, rem.EndDate.toString("dd.MM.yyyy"));
//     top->setText(3, rem.reasonName);
//     top->setText(4, rem.action);
//     top->setText(5, rem.defect);
//     top->setText(6, rem.remark);
//     top->setData(0, Qt::UserRole, rem.id);
//     if(rem.listRemStep.size() > 0)
//         top->setText(7, rem.listRemStep.last().status.name);
//     ui->trwRemont->addTopLevelItem(top);

//     for(auto const &it2 : rem.listRemStep)
//     {
//         QTreeWidgetItem *child = new QTreeWidgetItem();
//         child->setText(1, it2.dateStep.toString("dd.MM.yyyy"));
//         child->setText(2, it2.status.name);
//         child->setText(3, it2.Comment);
//         top->addChild(child);
//     }

// }



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

void CardProdWindow::LoadRemontToForm(QList<Remont> &listRemont)
{
    // ui->twRemont->setRowCount(0);
    ui->twRemont->setRowCount(listRemont.size());

    int row = 0;
    for(auto &it : listRemont)
    {
        Claim claim = repo.GetClaim(it.idReclamation);
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
// Загрузка набора для выбраннного изделия или модуля
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::loadShipmentToForm(const Product *prod)
{
    SetterOut setter = repo.GetSetter(prod->idSetterOut);
    Shipment ship = repo.GetShipment(setter.idShipment);

    repo.LoadChildSetter(setter);
    for(auto &it: setter.listProduct)
    {
        repo.LoadChildProduct(it);
    }

    ui->lbContract->setText(ship.schet);
    ui->lbCardOrder->setText(ship.cardOrder);
    ui->lbObjectInstall->setText(ship.objectInstall);
    ui->lbProduction->setText(prod->name);
    ui->lbDateUPD->setText(ship.dateUPD.toString("dd.MM.yyyy"));
    ui->lbNumberUPD->setText(ship.numberUPD);

    QTreeWidgetItem *top = new QTreeWidgetItem();
    top->setText(0, setter.name);
    ui->treeWidget->addTopLevelItem(top);
    top->setExpanded(true);
    for(auto const &it : setter.listProduct)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        QString s = it.name + "(" + it.number + ")";
        if(it.getIsRepair())
            s += " неисправен";
        child->setText(0, s);
        if(it.number == number)
        {
            QFont font;
            font.setBold(true);
            child->setFont(0, font);
        }
        top->addChild(child);
        child->setExpanded(true);
        for(auto mod : it.listModules)
        {
            // Modul modul = mod;
            mod.LoadStatus(mod);
            QTreeWidgetItem *modItem = new QTreeWidgetItem();
            s = mod.name + "(" + mod.number + ")";
            if(mod.getIsRepair())
                s += " неисправен";
            modItem->setText(0, s);
            if(mod.number == number)
            {
                QFont font;
                font.setBold(true);
                modItem->setFont(0, font);
            }
            child->addChild(modItem);
        }
    }
}

//-------------------------------------------------------------------------------------------------------
// Загрузка состава изделия
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::loadInclude(const Product *prod)
{
    for(auto const &mod : prod->listModules)
    {
        QListWidgetItem *modItem = new QListWidgetItem();
        modItem->setText(mod.name + "(" + mod.number + ")");
        ui->lwInclude->addItem(modItem);
    }
}


//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------

