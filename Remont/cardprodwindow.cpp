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
    setWindowTitle("Карточка \"" + name + "\"");

}


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

    if(setter.id <= 0)
        return;

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
    top->setIcon(0, QIcon("://image/setter.png"));
    ui->treeWidget->addTopLevelItem(top);
    top->setExpanded(true);
    for(auto const &it : setter.listProduct)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setIcon(0, QIcon("://image/product.png"));
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
            modItem->setIcon(0, QIcon("://image/modul.png"));
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

