#include "otkcontrolwindow.h"
#include "scan.h"
#include "ui_otkcontrolwindow.h"

#include <QInputDialog>
#include <QKeyEvent>

OTKControlWindow::OTKControlWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OTKControlWindow)
{
    ui->setupUi(this);

    // получить списки изделий и модулей, требующих проверку
    loadCreatedDevice();
    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

OTKControlWindow::~OTKControlWindow()
{
    disconnect(conn);
    delete ui;
}


//---------------------------------------------------------------------------------------
// Загрузка исправных устройств
//---------------------------------------------------------------------------------------
void OTKControlWindow::loadCreatedDevice()
{
    Modules.FindItems("", Status::CREATE);

    ui->lwModul->clear();
    for(auto &it : Modules.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(/*it.name + " №" + */it.FullNameAndComment());
        item->setData(Qt::UserRole, it.id);
        item->setData(Qt::UserRole + 1, it.number);
        ui->lwModul->addItem(item);
    }

    Products.FindItems("", Status::CREATE);
    ui->lwProduct->clear();
    for(auto &it : Products.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(/*it.name + " №" + */it.FullNameAndComment());
        item->setData(Qt::UserRole, it.id);
        item->setData(Qt::UserRole + 1, it.number);
        ui->lwProduct->addItem(item);
    }

    Plates.FindItems("", Status::CREATE);
    ui->lwPlate->clear();
    for(auto &it : Plates.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it.VNFT + " №" + it.number);
        item->setData(Qt::UserRole, it.id);
        item->setData(Qt::UserRole + 1, it.number);
        ui->lwPlate->addItem(item);
    }

}


//---------------------------------------------------------------------------------------
// Загрузка неисправных устройств
//---------------------------------------------------------------------------------------
void OTKControlWindow::loadBrockenDevice()
{
    Modules.FindItems("", Status::FAULTY);

    ui->lwModul->clear();
    for(auto &it : Modules.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(/*it.name + " №" + */it.FullNameAndComment());
        item->setData(Qt::UserRole, it.id);
        item->setData(Qt::UserRole + 1, it.number);
        ui->lwModul->addItem(item);
    }

    Products.FindItems("", Status::FAULTY);
    // repo.LoadProducts(listProduct, Status::FAULTY);
    ui->lwProduct->clear();
    for(auto &it : Products.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(/*it.name + " №" + */it.FullNameAndComment());
        item->setData(Qt::UserRole, it.id);
        item->setData(Qt::UserRole + 1, it.number);
        ui->lwProduct->addItem(item);
    }

}


// void OTKControlWindow::keyPressEvent(QKeyEvent *event)
// {
//     if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
//     {
//         if(!scanNumber.isEmpty())
//         {

//         }

//         scanNumber.clear();
//     }
//     else
//     {
//         scanNumber += event->text();
//     }

// }


//---------------------------------------------------------------------------------------
// Кнопка Модуль Удалить из списка прошедших проверку
//---------------------------------------------------------------------------------------
void OTKControlWindow::on_tbDelCheck_clicked()
{
    QListWidgetItem *item = ui->lwChecked->currentItem();
    if(item == nullptr)
        return;

    int idModul = item->data(Qt::UserRole).toInt();
    QListWidgetItem *item2 = new QListWidgetItem(*item);
    ui->lwModul->addItem(item2);

    listStatus.remove(idModul);

    delete item;
}


//---------------------------------------------------------------------------------------
// Кнопка Модуль Удалить из списка не прошедших проверку
//---------------------------------------------------------------------------------------
void OTKControlWindow::on_tbDelBroken_clicked()
{
    QListWidgetItem *item = ui->lwBroken->currentItem();
    if(item == nullptr)
        return;

    int idModul = item->data(Qt::UserRole).toInt();
    QListWidgetItem *item2 = new QListWidgetItem(*item);
    auto modul = std::find_if(Modules.listItems.cbegin(), Modules.listItems.cend(), [&] (const Modul p) { return p.id == idModul;});
    Modul mod = *modul;
    item2->setText(mod.FullNameAndComment());
    ui->lwModul->addItem(item2);

    listStatus.remove(idModul);
    delete item;

}


//---------------------------------------------------------------------------------------
// Кнопка Изелие Удалить из списка прошедших проверку
//---------------------------------------------------------------------------------------
void OTKControlWindow::on_tbDelCheckProd_clicked()
{
    QListWidgetItem *item = ui->lwCheckedProd->currentItem();
    if(item == nullptr)
        return;

    int idProd = item->data(Qt::UserRole).toInt();
    QListWidgetItem *item2 = new QListWidgetItem(*item);
    ui->lwProduct->addItem(item2);

    listStatusProd.remove(idProd);

    delete item;
}


//---------------------------------------------------------------------------------------
// Кнопка Изелие Удалить из списка не прошедших проверку
//---------------------------------------------------------------------------------------
void OTKControlWindow::on_tbDelBrokenProd_clicked()
{
    QListWidgetItem *item = ui->lwBrokenProd->currentItem();
    if(item == nullptr)
        return;

    int idProd = item->data(Qt::UserRole).toInt();
    QListWidgetItem *item2 = new QListWidgetItem(*item);
    auto prod = std::find_if(Products.listItems.cbegin(), Products.listItems.cend(), [&] (const Product p) { return p.id == idProd;});
    Product product = *prod;
    item2->setText( product.FullNameAndComment());
    ui->lwProduct->addItem(item2);

    listStatusProd.remove(idProd);
    delete item;

}

void OTKControlWindow::on_tbDelCheckPlate_clicked()
{
    QListWidgetItem *item = ui->lwCheckedPlate->currentItem();
    if(item == nullptr)
        return;

    int id = item->data(Qt::UserRole).toInt();
    QListWidgetItem *item2 = new QListWidgetItem(*item);
    ui->lwPlate->addItem(item2);

    listStatusPlate.remove(id);

    delete item;

}

void OTKControlWindow::on_tbDelBrokenPlate_clicked()
{
    QListWidgetItem *item = ui->lwBrokenPlate->currentItem();
    if(item == nullptr)
        return;

    int id = item->data(Qt::UserRole).toInt();
    QListWidgetItem *item2 = new QListWidgetItem(*item);
    Plate plate = Plates.GetItem(id);
    // auto plate = std::find_if(Plates.listItems.cbegin(), Plates.listItems.cend(), [&] (const Plate p) { return p.id == id;});
    // Plate plate2 = plate;
    item2->setText( plate.FullNameAndComment());
    ui->lwPlate->addItem(item2);

    listStatusPlate.remove(id);
    delete item;

}


//---------------------------------------------------------------------------------------
// подтверждение записи в базу при закрытии окна
//---------------------------------------------------------------------------------------
void OTKControlWindow::on_OTKControlWindow_accepted()
{
    for(auto &it : listStatus)
    {
        Modul mod;
        mod.id = it.idDevice;
        mod.AddStatus(mod, it.idStatus, it.Comment);
    }

    for(auto &it : listStatusProd)
    {
        Product prod;
        prod.id = it.idDevice;
        prod.AddStatus(prod, it.idStatus, it.Comment);
    }
}


void OTKControlWindow::on_rbOldDevice_toggled(bool checked)
{
    if(checked)
        loadBrockenDevice();
    else
        loadCreatedDevice();
}


//---------------------------------------------------------------------------------------
// Событие сканирования номера
//---------------------------------------------------------------------------------------
void OTKControlWindow::slotReadScan(QString s)
{
    if(!isActiveWindow())
        return;

    Status status;
    status.idStatus = Status::CORRECT;
    status.dateStatus = QDateTime::currentDateTime();

    int indexTab = ui->tabWidget->currentIndex();
    QListWidgetItem *item;

    switch(indexTab)
    {
    case 0:
        for(int row = 0; row < ui->lwProduct->count(); ++row)
        {
            item = ui->lwProduct->item(row);
            if(item->data(Qt::UserRole + 1).toString() == s)
            {
                ItemCheckedControl(item);
                break;
            }
        }
        break;
    case 1:
        for(int row = 0; row < ui->lwModul->count(); ++row)
        {
            item = ui->lwModul->item(row);
            if(item->data(Qt::UserRole + 1).toString() == s)
            {
                ItemCheckedControl(item);
                break;
            }
        }
        break;
    case 2:
        for(int row = 0; row < ui->lwPlate->count(); ++row)
        {
            item = ui->lwPlate->item(row);
            if(item->data(Qt::UserRole + 1).toString() == s)
            {
                ItemCheckedControl(item);
                break;
            }
        }
        break;
    }

}


//---------------------------------------------------------------------------------------
// Добавление выбранного элемента из списка изделий или модулей
//---------------------------------------------------------------------------------------
void OTKControlWindow::ItemCheckedControl(QListWidgetItem *item)
{
    if(item == nullptr)
        return;

    QListWidgetItem *item2;
    Status status;
    int id;

    int index = ui->tabWidget->currentIndex();

    switch(index)
    {
    case 0:
    {
        item2 = new QListWidgetItem(*item);
        id = item->data(Qt::UserRole).toInt();
        Product prod = Products.GetItem(id);

        status.idDevice = id;
        status.dateStatus = QDateTime::currentDateTime();

        // контроль пройден
        if(ui->rbCheck->isChecked())
        {
            status.idStatus = Status::CORRECT;
            ui->lwCheckedProd->addItem(item2);
        }
        // контроль не пройден
        else
        {
            QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");
            status.Comment = comment;
            status.idStatus = Status::FAULTY;

            item2->setText(prod.name + " " + prod.number + " (" + comment + ")");
            ui->lwBrokenProd->addItem(item2);
        }


        prod.listStatus.push_back(status);
        listStatusProd[id] = status;
        delete item;
    }
        break;

    case 1:
    {
        item2 = new QListWidgetItem(*item);
        id = item->data(Qt::UserRole).toInt();
        Modul mod = Modules.GetItem(id);

        status.idDevice = id;
        status.dateStatus = QDateTime::currentDateTime();

        if(ui->rbCheck->isChecked())
        {
            status.idStatus = Status::CORRECT;
            ui->lwChecked->addItem(item2);
        }
        else
        {
            QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");
            status.idStatus = Status::FAULTY;
            status.Comment = comment;
            item2->setText(mod.name + " " + mod.number + " (" + comment + ")");
            ui->lwBroken->addItem(item2);
        }
        listStatus[id] = status;;
        mod.listStatus.push_back(status);
        delete item;
    }
        break;

    case 2:
    {
        item2 = new QListWidgetItem(*item);
        id = item->data(Qt::UserRole).toInt();
        Plate plate = Plates.GetItem(id);

        status.idDevice = id;
        status.dateStatus = QDateTime::currentDateTime();

        if(ui->rbCheck->isChecked())
        {
            status.idStatus = Status::CORRECT;
            ui->lwCheckedPlate->addItem(item2);
        }
        else
        {
            QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");
            status.idStatus = Status::FAULTY;
            status.Comment = comment;
            item2->setText(plate.name + " " + plate.number + " (" + comment + ")");
            ui->lwBrokenPlate->addItem(item2);
        }
        listStatus[id] = status;;
        plate.listStatus.push_back(status);
        delete item;
    }
        break;
    }
}



//---------------------------------------------------------------------------------------
// Кнопка Проверено
//---------------------------------------------------------------------------------------
void OTKControlWindow::on_pbChecked_clicked()
{
    int tabIndex = ui->tabWidget->currentIndex();

    QListWidgetItem *item;
    switch(tabIndex)
    {
    case 0:
        item = ui->lwProduct->currentItem();
        break;
    case 1:
        item = ui->lwModul->currentItem();
        break;
    case 2:
        item = ui->lwPlate->currentItem();
        break;
    }

    ItemCheckedControl(item);

}

