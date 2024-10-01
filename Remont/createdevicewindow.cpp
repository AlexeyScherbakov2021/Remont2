#include "createdevicewindow.h"
#include "selectplatewindow.h"
#include "ui_createdevicewindow.h"

#include <models/listmodul.h>
#include <models/listplate.h>
#include <models/modul.h>
#include <models/product.h>

CreateDeviceWindow::CreateDeviceWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateDeviceWindow)/*, selectIdPlate(0)*/
{
    ui->setupUi(this);

    repo.LoadModuleType(listTypeModule);
    repo.LoadProductType(listTypeProduct);

    for(auto it = listTypeModule.cbegin(); it != listTypeModule.cend(); ++it)
        ui->cbModul->addItem(*it, it.key());

    for(auto it = listTypeProduct.cbegin(); it != listTypeProduct.cend(); ++it)
        ui->cbProduct->addItem(*it, it.key());


}

CreateDeviceWindow::~CreateDeviceWindow()
{
    delete ui;
}



//---------------------------------------------------------------------------------
// Удаление модуля
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_tbDeleteModul_clicked()
{
    // QTreeWidgetItem *item = ui->twModul->currentItem();
    // if(item->parent() != nullptr)
    //     item = item->parent();

    // int id = item->data(0, Qt::UserRole).toInt();
    // if(repo.DeleteModul(id))
    //     delete item;
}


//---------------------------------------------------------------------------------
// Выбор платы по серийному номеру
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_tbSearchPlate_clicked()
{
    // QString s = ui->leSearchPlate->text();
    // ListPlate listPlate;

    // // Убрать уже добавленные платы
    // listPlate.FindItems(s, listPlate);

    // if(listPlate.size() == 0)
    //     return;

    // int currentIndex = 0;

    // if(listPlate.size() > 1)
    // {
    //     SelectPlateWindow win(listPlate, this);
    //     if(win.exec() == QDialog::Accepted)
    //         currentIndex = win.selectedIndex;
    //     else
    //         return;
    // }

    // Plate plate = listPlate.at(currentIndex);

    // // Сделать проверку на повтор

    // listAddingPlate.push_back(plate);

    // addLinePlate(&plate);
    // ui->twPlates->resizeColumnsToContents();
    // ui->twPlates->resizeRowsToContents();


}



//---------------------------------------------------------------------------------
// Удаление изделия
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_tbDeleteProduct_clicked()
{
    // int id = ui->lwProduct->currentItem()->data(Qt::UserRole).toInt();
    // if(repo.DeleteProduct(id) )
    //     delete ui->lwProduct->currentItem();

}



//---------------------------------------------------------------------------------
// Добавление строки в список плат
//---------------------------------------------------------------------------------
void CreateDeviceWindow::addLinePlate(Plate *plate)
{
    int row = ui->twPlates->rowCount();
    ui->twPlates->insertRow(row);

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(plate->number);
    item->setData(Qt::UserRole, plate->id);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 0, item);

    item = new QTableWidgetItem();
    item->setText(plate->number2);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 1, item);

    item = new QTableWidgetItem();
    item->setText(plate->numberDoc);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 2, item);

    item = new QTableWidgetItem();
    item->setText(plate->VNFT);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 3, item);

    item = new QTableWidgetItem();
    item->setText(plate->dateRegister.toString("dd.MM.yyyy"));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 4, item);

}



//---------------------------------------------------------------------------------
// Добавление строки в список модулей
//---------------------------------------------------------------------------------
void CreateDeviceWindow::addLineModul(Modul &mod)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, mod.number);
    item->setData(0, Qt::UserRole, mod.id);
    ui->twModul->addTopLevelItem(item);

    for(const auto &it : mod.listPlate)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(0, it.number + " (FW: " + it.number2 + ")");
        item->addChild(child);
    }
    item->setExpanded(true);

}


//---------------------------------------------------------------------------------
// Кнопка Регистрации изделия
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_pbRegProduct_clicked()
{
    if(ui->leNumProduct->text().isEmpty())
        return;

    Product prod;
    // Добавление изделия в  базу данных со статусом Создан
    prod.number = ui->leNumProduct->text();
    prod.name = ui->leNameProd->text();
    prod.prodTypeId = ui->cbProduct->currentData(Qt::UserRole).toInt();
    prod.dateRegister = QDateTime::currentDateTime();
    if(repo.AddItem(prod))
    {
        Status status;
        status.idDevice = prod.id;
        status.idStatus = Status::CREATE;
        status.dateStatus = QDateTime::currentDateTime();
        prod.AddStatus(prod, status, &repo);
        // repo.AddStatusProduct(status);

        QString s = ui->cbProduct->currentText();
        QListWidgetItem *item = new QListWidgetItem(ui->leNumProduct->text() + " (" + s + ")");
        item->setData(Qt::UserRole, prod.id);
        ui->lwProduct->addItem(item);
        ui->leNumProduct->clear();
        ui->leNumProduct->setFocus();
    }

}


//---------------------------------------------------------------------------------
// Кнопка Регистрации модуля
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_pbRegModul_clicked()
{
    if(ui->leNumModul->text().isEmpty())
        return;

    ListModul lModul;
    Modul mod;
    // добавить устройство в базу со статусом Создан
    mod.number = ui->leNumModul->text();
    mod.name = ui->leModulName->text();
    mod.idType = ui->cbModul->currentData(Qt::UserRole).toInt();
    // mod.numberFW = ui->lbNumberFWPlate->text();
    mod.dateRegister = QDateTime::currentDateTime();
    mod.listPlate = listAddingPlate;
    if(repo.AddItem(mod))
    {
        Status status;
        status.idDevice = mod.id;
        status.idStatus = Status::CREATE;
        status.dateStatus = QDateTime::currentDateTime();
        mod.AddStatus(mod, status, &repo);
        // repo.AddStatusModul(status);
        for(auto &it : mod.listPlate)
            lModul.LinkPlate(it.id, mod.id);
            // repo.LinkPlate(it.id, mod.id);

        addLineModul(mod);
        ui->leNumModul->clear();
        ui->leNumModul->setFocus();
    }

    listAddingPlate.clear();
    ui->twPlates->clear();
}


void CreateDeviceWindow::on_tbDelPlate_clicked()
{
    // QTableWidgetItem *item = ui->twPlates->currentItem();
    // if(item == nullptr)
    //     return;

    // int id = item->data(Qt::UserRole).toInt();

    int row = ui->twPlates->currentRow();
    if(row < 0)
        return;

    listAddingPlate.removeAt(row);
    ui->twPlates->removeRow(row);

    // auto plate_iter = std::find_if(listAddingPlate.begin(), listAddingPlate.end(), [id](Plate &p) { return p.id == id;});
    // std::remove_if(listAddingPlate.begin(), listAddingPlate.end(), [id](Plate &p) { return p.id == id;});

}

