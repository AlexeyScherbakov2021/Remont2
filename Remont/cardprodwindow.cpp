#include "cardprodwindow.h"
#include "ui_cardprodwindow.h"
#include <models/listproduct.h>
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

    if(device->typeDevice == ev::MODUL)
    {
        Modul *mod = static_cast<Modul*>(device);
        product = repo.GetProduct(mod->idProduct);
        prod = &product;
        mod->LoadStatus(*mod);
        LoadHistoryToForm(mod->listStatus);
    }
    if(device->typeDevice == ev::PRODUCT)
    {
        prod = static_cast<Product*>(device);
        repo.LoadChildProduct(*prod);
        prod->LoadStatus(*prod);
        loadInclude(prod);
        LoadHistoryToForm(prod->listStatus);

    }

    ui->lbGarant->setText(device->EndGarant.toString("dd.MM.yyyy"));
    ui->lbDateCreate->setText(device->dateRegister.toString("dd.MM.yyyy"));
    ui->lbNumber->setText(device->number);

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
void CardProdWindow::LoadHistoryToForm(QList<Status> listStatus)
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
        child->setText(0, it.name + "(" + it.number + ")");
        if(it.number == number)
        {
            QFont font;
            font.setBold(true);
            child->setFont(0, font);
        }
        top->addChild(child);
        child->setExpanded(true);
        for(auto const &mod : it.listModules)
        {
            QTreeWidgetItem *modItem = new QTreeWidgetItem();
            modItem->setText(0, mod.name + "(" + mod.number + ")");
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
// Кнопка добавления ремонта
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::on_pbAddRemont_clicked()
{
    // RemontWindow *win = new RemontWindow(this);
    // if(win->exec() == QDialog::Accepted)
    //     AddRowRemont(remontEntity->listRemont->last());

}


//-------------------------------------------------------------------------------------------------------
// Кнопка Изменение статуса
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::on_pbEditStatus_clicked()
{
    // найти текущий ремонт в модуле
    // auto res = std::find_if(modul.listRemont.cbegin(), modul.listRemont.cend(),
    //                         [] (const RemontM &step) { return step.EndDate.isNull(); });

    // if(res == modul.listRemont.cend())
    //     return;


    // auto res = std::find_if(remontEntity->listRemont->cbegin(), remontEntity->listRemont->cend(),
    //                         [] (const RemontM &step) { return step.EndDate.isNull(); });

    // if(res == remontEntity->listRemont->cend())
    //     return;

    // RemontM rem = *res;

    // QTreeWidgetItem *top = nullptr;
    // for(int row = 0; row < ui->trwRemont->topLevelItemCount(); ++row)
    // {
    //     if(ui->trwRemont->topLevelItem(row)->data(0, Qt::UserRole).toInt() == rem.id)
    //     {
    //         top = ui->trwRemont->topLevelItem(row);
    //         break;
    //     }
    // }

    // if(top == nullptr)
    //     return;

    // RemontStatusWindow *win = new RemontStatusWindow(&rem, this);
    // if(win->exec() == QDialog::Accepted)
    // {
    //     RemontStep step = rem.listRemStep.last();
    //     QTreeWidgetItem *child = new QTreeWidgetItem();
    //     child->setText(1, step.dateStep.toString("dd.MM.yyyy"));
    //     child->setText(2, step.status.name);
    //     child->setText(3, step.Comment);
    //     top->addChild(child);
    //     top->setText(7, step.status.name);
    //     top->setText(2, rem.EndDate.toString("dd.MM.yyyy"));

    //     remontEntity->AddRemontStep(step, &repo);
    //     // repo.AddRemontStep(step);
    // }
}


//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------

