#include <QInputDialog>
#include <QMessageBox>
#include "selectdevicewindow.h"
#include "shipwindow.h"
#include "ui_shipwindow.h"

ShipWindow::ShipWindow(Shipment *shipment, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ShipWindow), ship(shipment)
{
    ui->setupUi(this);

    repo.LoadOrganization(listOrg);
    for(auto it = listOrg.begin(); it != listOrg.end(); ++it )
    {
        ui->cbCusomer->addItem(*it, it.key());
    }

    if(ship->id != 0)
    {
        ui->leBuyer->setText(ship->buyer);
        ui->leCardOrder->setText(ship->cardOrder);
        // ui->leCustomer->setText(ship->customer);
        ui->leNumUPD->setText(ship->numberUPD);
        ui->leObjectInstall->setText(ship->objectInstall);
        ui->leSchet->setText(ship->schet);
        ui->deDateUPD->setDateTime(ship->dateUPD);

        repo.LoadShipSetter(ship->listSetterOut, ship->id);
        for(auto &it : ship->listSetterOut)
        {
            QTreeWidgetItem *itemSet = AddItemTree(it.name, it.id, TypeItemTree::SET);

            repo.LoadChildSetter(it);
            for(auto &itProd : it.listProduct)
                AddItemProd(itProd, itemSet);
        }

        repo.LoadShipProduct(ship->listProduct, ship->id);
        for(auto &it : ship->listProduct)
            AddItemProd(it);

        repo.LoadShipModule(ship->listModules, ship->id);
        for(auto &it : ship->listModules)
            AddItemTree(it.name + " (" + it.number + ")", it.id, TypeItemTree::MODUL);
    }


}

ShipWindow::~ShipWindow()
{
    delete ui;
}


//-----------------------------------------------------------------------------------
// Добавление изделия в отгрузку
//-----------------------------------------------------------------------------------
void ShipWindow::on_tbNumProd_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumProd->text(),
            Status::CORRECT, SelectDeviceWindow::TypeDevice::TypeProduct);

    if(win->exec() == QDialog::Accepted)
    {
        if(ship->id == 0)
            repo.AddItem(*ship);

        win->prod.idShipment = ship->id;
        if(repo.UpdateItem(win->prod))
            AddItemProd(win->prod);
    }
}

//-----------------------------------------------------------------------------------
// Добавление изделия в набор и отгрузку
//-----------------------------------------------------------------------------------
void ShipWindow::on_tbAddSetterProd_clicked()
{
    QTreeWidgetItem *item = ui->twTreeDevice->currentItem();

    if(item != nullptr)
    {
        TypeItemTree type = (TypeItemTree)item->data(0, Qt::UserRole + 1).toInt();
        if(type != TypeItemTree::SET)
            return;
    }

    SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumProd->text(),
                        Status::CORRECT, SelectDeviceWindow::TypeDevice::TypeProduct);

    if(win->exec() == QDialog::Accepted)
    {
        if(ship->id == 0)
            repo.AddItem(*ship);

        if(item == nullptr)
        {
            // Добавление набора в класс Shipment
            SetterOut setter;
            setter.name = "_" + win->prod.name;
            setter.idShipment = ship->id;
            setter.listProduct.push_back(win->prod);
            if(repo.AddItem(setter))
                item = AddItemTree(setter.name, setter.id, TypeItemTree::SET);
            win->prod.idSetterOut = setter.id;
            ship->listSetterOut.push_back(setter);
        }
        else
        {
            win->prod.idSetterOut = item->data(0, Qt::UserRole).toInt();
            // shipLog.listSetterOut
        }

        if(repo.UpdateItem(win->prod))
            AddItemProd(win->prod, item);
        // Добавление изделия в класс Shipment
    }

}

//-----------------------------------------------------------------------------------
// добавления в дерево экранной формы изделия
//-----------------------------------------------------------------------------------
QTreeWidgetItem* ShipWindow::AddItemProd(Product &prod, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *itemProd = AddItemTree(prod.name + " (" + prod.number + ")", prod.id, TypeItemTree::PRODUCT, parent);

    if(itemProd != nullptr)
    {
        // Включение изделия в класс Shipment
        repo.LoadChildProduct(prod);
        for(auto &it : prod.listModules)
            AddItemTree(it.name + " (" + it.number + ")", it.id, TypeItemTree::MODUL, itemProd);
    }

    return itemProd;
}



//-----------------------------------------------------------------------------------
// Добавление модуля в отгрузку
//-----------------------------------------------------------------------------------
void ShipWindow::on_tbNumModul_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumModul->text(),
                                                     Status::CORRECT, SelectDeviceWindow::TypeDevice::TypeModul);

    if(win->exec() == QDialog::Accepted)
    {
        Modul mod = win->modul;
        AddItemTree(mod.name + " (" + mod.number + ")", mod.id, TypeItemTree::MODUL);
        // Добавление модуля в класс Shipment
        ship->listModules.push_back(mod);

        if(ship->id == 0)
            repo.AddItem(*ship);

        mod.idShipment = ship->id;
        repo.UpdateItem(mod);
    }
}

//-----------------------------------------------------------------------------------
// Кнопка добавления нового набора
//-----------------------------------------------------------------------------------
void ShipWindow::on_pbAddSet_clicked()
{
    bool ok;
    QString res = QInputDialog::getText(this, tr("Ввод текста"), tr("Наименование набора:"), QLineEdit::Normal,
        "Новый набор", &ok);

    if(ok && !res.isEmpty())
    {
        AddItemTree(res, 0, TypeItemTree::SET);
        // Добавление набора в класс Shipment
        SetterOut setter;
        setter.name = res;

        if(ship->id == 0)
            repo.AddItem(*ship);

        setter.idShipment = ship->id;
        if(repo.AddItem(setter))
            ship->listSetterOut.push_back(setter);

    }
}


//-----------------------------------------------------------------------------------
// добавления в дерево экранной формы строки
//-----------------------------------------------------------------------------------
QTreeWidgetItem* ShipWindow::AddItemTree(QString text, int id, TypeItemTree type, QTreeWidgetItem *parent)
{
    if(listID.contains(id * 10 + type))
        return nullptr;

    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, text);
    item->setData(0, Qt::UserRole, id);
    item->setData(0, Qt::UserRole + 1, type);
    if(parent == nullptr)
    {
        ui->twTreeDevice->addTopLevelItem(item);
        listID.insert(id * 10 + type, 0);
    }
    else
        parent->addChild(item);
    item->setExpanded(true);


    return item;
}


//-----------------------------------------------------------------------------------
// Удаление ветки в дереве
//-----------------------------------------------------------------------------------
void ShipWindow::on_pbDelete_clicked()
{
    QTreeWidgetItem *item = ui->twTreeDevice->currentItem();
    if(item == nullptr)
        return;

    if(item->parent() != nullptr)
    {
        TypeItemTree type = (TypeItemTree)item->parent()->data(0, Qt::UserRole + 1).toInt();
        if(type != TypeItemTree::SET)
            return;

    }

    SetterOut setter;
    setter.id = item->data(0, Qt::UserRole).toInt();
    TypeItemTree type = (TypeItemTree)item->data(0, Qt::UserRole + 1).toInt();

    bool res = false;
    switch(type)
    {
        case TypeItemTree::PRODUCT:
            {
                auto prod_it = std::find_if(ship->listProduct.cbegin(), ship->listProduct.cend(),
                                            [&](const Product &p) { return p.id == setter.id; });

                if(prod_it != ship->listProduct.cend())
                {
                    Product prod = *prod_it;
                    prod.idShipment = 0;
                    res = repo.UpdateItem(prod);
                }
            }
            break;

        case TypeItemTree::MODUL:
            {
                auto mod_it = std::find_if(ship->listModules.cbegin(), ship->listModules.cend(),
                                           [&](const Modul &m) { return m.id == setter.id; });
                if(mod_it != ship->listModules.cend())
                {
                    Modul modul = *mod_it;
                    modul.idShipment = 0;
                    res = repo.UpdateItem(modul);
                }
            }
            break;
        case TypeItemTree::SET:
            res = repo.DeleteItem(setter);
            break;
    }

    listID.remove(setter.id * 10 + type);

    delete item;
}


//-----------------------------------------------------------------------------------
// Кнопка Отгрузить
//-----------------------------------------------------------------------------------
void ShipWindow::on_pbFinish_clicked()
{

    ship->dateRegister = QDateTime::currentDateTime();

    for(auto &it : ship->listSetterOut)
    {
        SetStatusProduct(it.listProduct);
    }

    SetStatusProduct(ship->listProduct);
    SetStatusModules(ship->listModules);


    // if(!repo.AddShipment(ship))
    //     QMessageBox::critical(this, "Ошибка", "Ошибка при записи в базу данных");
    // else
    // {
    //     for(auto &it : ship->listSetterOut)
    //         SetStatusProduct(it.listProduct);

    //     SetStatusProduct(ship->listProduct);
    //     SetStatusModules(ship->listModules);
    // }

    accept();
}

//-----------------------------------------------------------------------------------
// Добавление нового статуса для изделия и его модулей
//-----------------------------------------------------------------------------------
void ShipWindow::SetStatusProduct(QList<Product> &listProduct)
{
    Status status;
    status.dateStatus = QDateTime::currentDateTime();
    status.idStatus = Status::Stat::SHIPPED;

    for(auto &itProd : listProduct)
    {
        status.idDevice = itProd.id;
        itProd.AddStatus(itProd, status);
        // repo.AddStatusProduct(status);

        SetStatusModules(itProd.listModules);
    }
}

//-----------------------------------------------------------------------------------
// Добавление нового статуса для модулей
//-----------------------------------------------------------------------------------
void ShipWindow::SetStatusModules(QList<Modul> &listModules)
{
    Status status;
    status.dateStatus = QDateTime::currentDateTime();
    status.idStatus = Status::Stat::SHIPPED;

    for(auto &itMod : listModules)
    {
        status.idDevice = itMod.id;
        itMod.AddStatus(itMod, status);
        // repo.AddStatusModul(status);
    }
}


//-----------------------------------------------------------------------------------
// Событие закрытия окна
//-----------------------------------------------------------------------------------
void ShipWindow::on_ShipWindow_finished(int result)
{
    ship->buyer = ui->leBuyer->text();
    ship->cardOrder = ui->leCardOrder->text();
    // ship->customer = ui->leCustomer->text();
    ship->numberUPD = ui->leNumUPD->text();
    ship->objectInstall = ui->leObjectInstall->text();
    ship->schet = ui->leSchet->text();
    ship->dateUPD = ui->deDateUPD->dateTime();

    repo.UpdateItem(*ship);

}

