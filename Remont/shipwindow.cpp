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
        ui->cbCusomer->addItem(*it, it.key());

    if(ship->id != 0)
    {
        // была выбрана существующая отгрузка
        ui->leBuyer->setText(ship->buyer);
        ui->leCardOrder->setText(ship->cardOrder);
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
    else
    {
        ui->deDateUPD->setDateTime(QDateTime::currentDateTime());
    }

    connect(ui->leBuyer, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leCardOrder, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leNumModul, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leNumUPD, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leObjectInstall, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leSchet, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
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
    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    win->setTypeSearch(SelectDeviceWindow::TypeDevice::TypeProduct);
    win->setNotShipped();
    IDevice *dev = win->SelectDevice(true, ui->leNumProd->text(),Status::CORRECT);
    if(dev != nullptr)
    {
        if(ship->id == 0)
            repo.AddItem(*ship);

        Product* prod = static_cast<Product*>(dev);
        prod->idShipment = ship->id;
        ship->listProduct.push_back(*prod);
        if(repo.UpdateItem(*prod))
            AddItemProd(*prod);
    }
}

//-----------------------------------------------------------------------------------
// Добавление изделия в набор
//-----------------------------------------------------------------------------------
void ShipWindow::on_tbAddSetterProd_clicked()
{
    QTreeWidgetItem *item = ui->twTreeDevice->currentItem();

    if(item == nullptr)
        return;

    // if(item != nullptr)
    // {
        TypeItemTree type = (TypeItemTree)item->data(0, Qt::UserRole + 1).toInt();
        if(type != TypeItemTree::SET)
            return;
    // }

    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    win->setTypeSearch(SelectDeviceWindow::TypeDevice::TypeProduct);
    win->setNotShipped();
    IDevice *dev = win->SelectDevice(true, ui->leNumProd->text(), Status::CORRECT);

    if(dev != nullptr)
    {
        if(ship->id == 0)
            repo.AddItem(*ship);

        Product* prod = static_cast<Product*>(dev);

        // if(item == nullptr)
        // {
        //     // Добавление набора в класс Shipment
        //     SetterOut setter;
        //     setter.name = "_" + prod->name;
        //     setter.idShipment = ship->id;
        //     setter.listProduct.push_back(*prod);
        //     if(repo.AddItem(setter))
        //         item = AddItemTree(setter.name, setter.id, TypeItemTree::SET);
        //     prod->idSetterOut = setter.id;
        //     ship->listSetterOut.push_back(setter);
        // }
        // else
        // {
        prod->idSetterOut = item->data(0, Qt::UserRole).toInt();
        auto setter_it = std::find_if(ship->listSetterOut.begin(), ship->listSetterOut.end(), [prod](const SetterOut s) { return s.id == prod->idSetterOut; });
        if(setter_it != ship->listSetterOut.end())
            (*setter_it).listProduct.push_back(*prod);
        // qDebug() << "Добавлено изделие в набор id=" << prod->idSetterOut;
        // }

        if(repo.UpdateItem(*prod))
            AddItemProd(*prod, item);
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
    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    win->setTypeSearch(SelectDeviceWindow::TypeDevice::TypeModul);
    win->setNotShipped();
    IDevice *dev = win->SelectDevice(true, ui->leNumModul->text(), Status::CORRECT);
    if(dev != nullptr)
    {
        Modul* mod = static_cast<Modul*>(dev);
        AddItemTree(mod->name + " (" + mod->number + ")", mod->id, TypeItemTree::MODUL);
        // Добавление модуля в класс Shipment
        ship->listModules.push_back(*mod);

        if(ship->id == 0)
            repo.AddItem(*ship);

        mod->idShipment = ship->id;
        repo.UpdateItem(*mod);
    }
}

//-----------------------------------------------------------------------------------
// Кнопка добавления нового набора
//-----------------------------------------------------------------------------------
// void ShipWindow::on_pbAddSet_clicked()
// {
//     bool ok;
//     QString res = QInputDialog::getText(this, tr("Ввод текста"), tr("Наименование набора:"), QLineEdit::Normal,
//         "Новый набор", &ok);

//     if(ok && !res.isEmpty())
//     {
//         AddItemTree(res, 0, TypeItemTree::SET);
//         // Добавление набора в класс Shipment
//         SetterOut setter;
//         setter.name = res;

//         if(ship->id == 0)
//             repo.AddItem(*ship);

//         setter.idShipment = ship->id;
//         if(repo.AddItem(setter))
//             ship->listSetterOut.push_back(setter);

//     }
// }


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

    int id = item->data(0, Qt::UserRole).toInt();
    TypeItemTree type = (TypeItemTree)item->data(0, Qt::UserRole + 1).toInt();

    bool res = false;
    switch(type)
    {
        case TypeItemTree::PRODUCT:
            {
                Product prod;
                if(item->parent() == nullptr)
                {
                    // изделие находится в отгрузке
                    auto prod_it = std::find_if(ship->listProduct.cbegin(), ship->listProduct.cend(),
                                            [id](const Product &p) { return p.id == id; });
                    if(prod_it != ship->listProduct.cend())
                    {
                        prod = *prod_it;
                        ship->listProduct.removeIf([prod] (auto p) { return prod.id == p.id; });
                    }
                }
                else
                {
                    // изделие находится в наборе
                    int id_parent = item->parent()->data(0, Qt::UserRole).toInt();
                    auto set_it = std::find_if(ship->listSetterOut.begin(), ship->listSetterOut.end(),
                                               [id_parent](const SetterOut &s) { return s.id == id_parent; });

                    if(set_it != ship->listSetterOut.end())
                    {
                        auto prod_it = std::find_if((*set_it).listProduct.cbegin(), (*set_it).listProduct.cend(),
                                           [&](const Product &p) { return p.id == id; });
                        if(prod_it != (*set_it).listProduct.cend())
                        {
                            prod = *prod_it;
                            // SetterOut setter = *set_it;
                            (*set_it).listProduct.removeIf( [prod](const Product p) { return prod.id == p.id; } );
                        }
                    }
                }

                if(prod.id != 0)
                {
                    prod.idShipment = 0;
                    prod.idSetterOut = 0;
                    res = repo.UpdateItem(prod);
                }
            }
            break;

        case TypeItemTree::MODUL:
            {
                auto mod_it = std::find_if(ship->listModules.cbegin(), ship->listModules.cend(),
                                           [&](const Modul &m) { return m.id == id; });
                if(mod_it != ship->listModules.cend())
                {
                    Modul modul = *mod_it;
                    modul.idShipment = 0;
                    res = repo.UpdateItem(modul);
                    ship->listModules.removeIf([modul] (auto m) { return modul.id == m.id;});
                }
            }
            break;
        case TypeItemTree::SET:
            res = repo.DeleteSetter(id);
            break;
    }

    if(res)
    {
        listID.remove(id * 10 + type);
        delete item;
    }
}


//-----------------------------------------------------------------------------------
// Кнопка Отгрузить
//-----------------------------------------------------------------------------------
void ShipWindow::on_pbFinish_clicked()
{

    int countProd = 0;

    for(auto &it : ship->listSetterOut)
        countProd += it.listProduct.size();


    if(ship->listModules.size() == 0
        && ship->listProduct.size() == 0
        && countProd == 0)
    {
        QMessageBox::warning(this, "Предупреждение", "Не сформирован состав отгрузки.");
        return;
    }

    if(ui->leNumUPD->text().isEmpty())
    {
        QMessageBox::warning(this, "Предупреждение", "Для отгрузки необходимо указать документ УПД.");
        return;
    }


    ship->dateRegister = QDateTime::currentDateTime();

    for(auto &it : ship->listSetterOut)
    {
        SetStatusProduct(it.listProduct);
    }
    SetStatusProduct(ship->listProduct);
    SetStatusModules(ship->listModules);
    accept();
}

//-----------------------------------------------------------------------------------
// Добавление нового статуса для изделия и его модулей
//-----------------------------------------------------------------------------------
void ShipWindow::SetStatusProduct(QList<Product> &listProduct)
{
    for(auto &itProd : listProduct)
    {
        itProd.AddStatus(itProd, Status::SHIPPED);
        SetStatusModules(itProd.listModules);
    }
}

//-----------------------------------------------------------------------------------
// Добавление нового статуса для модулей
//-----------------------------------------------------------------------------------
void ShipWindow::SetStatusModules(QList<Modul> &listModules)
{
    for(auto &itMod : listModules)
        itMod.AddStatus(itMod, Status::SHIPPED);
}


//-----------------------------------------------------------------------------------
// Событие закрытия окна
//-----------------------------------------------------------------------------------
void ShipWindow::on_ShipWindow_finished(int /*result*/)
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


void ShipWindow::on_pbClose_clicked()
{
    if(isEditing)
        accept();
    else
        reject();
}

void ShipWindow::slotIsEditing()
{
    isEditing = true;
}


//-----------------------------------------------------------------------------------
// Кнопка добавления нового набора
//-----------------------------------------------------------------------------------
void ShipWindow::on_tbAddSetter_clicked()
{
    // bool ok;
    QString res = ui->leSetterOut->text();

    if(/*ok && */!res.isEmpty())
    {
        // Добавление набора в класс Shipment
        SetterOut setter;
        setter.name = res;

        if(ship->id == 0)
            repo.AddItem(*ship);

        setter.idShipment = ship->id;
        if(repo.AddItem(setter))
        {
            ship->listSetterOut.push_back(setter);
            AddItemTree(res, setter.id, TypeItemTree::SET);
            // qDebug() << "Добавлен набор id=" << setter.id;
        }
    }

}


void ShipWindow::on_tbAddSetProd_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    win->setTypeSearch(SelectDeviceWindow::TypeDevice::TypeProduct);
    win->setNotShipped();
    IDevice *dev = win->SelectDevice(true, ui->leNumProd->text(), Status::CORRECT);

    if(dev != nullptr)
    {
        QTreeWidgetItem *item = nullptr;
        if(ship->id == 0)
            repo.AddItem(*ship);

        Product* prod = static_cast<Product*>(dev);

        // Добавление набора в класс Shipment
        SetterOut setter;
        setter.name = "_" + prod->name;
        setter.idShipment = ship->id;
        setter.listProduct.push_back(*prod);
        if(repo.AddItem(setter))
            item = AddItemTree(setter.name, setter.id, TypeItemTree::SET);
        prod->idSetterOut = setter.id;
        ship->listSetterOut.push_back(setter);

        if(repo.UpdateItem(*prod))
            AddItemProd(*prod, item);
    }

}

