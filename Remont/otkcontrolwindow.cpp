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


void OTKControlWindow::loadCreatedDevice()
{
    Modules.FindItems("", Status::CREATE);
    // repo.LoadModulsStatus(listModul, Status::CREATE);

    ui->lwModul->clear();
    for(auto &it : Modules.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it.name + " №" + it.numAndComment());
        item->setData(Qt::UserRole, it.id);
        item->setData(Qt::UserRole + 1, it.number);
        ui->lwModul->addItem(item);
    }

    Products.FindItems("", Status::CREATE);
    // repo.LoadProducts(listProduct, Status::CREATE);
    ui->lwProduct->clear();
    for(auto &it : Products.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it.name + " №" + it.numAndComment());
        item->setData(Qt::UserRole, it.id);
        item->setData(Qt::UserRole + 1, it.number);
        ui->lwProduct->addItem(item);
    }
}


void OTKControlWindow::loadBrockenDevice()
{
    Modules.FindItems("", Status::FAULTY);
    // repo.LoadModulsStatus(listModul, Status::FAULTY);

    ui->lwModul->clear();
    for(auto &it : Modules.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it.name + " №" + it.numAndComment());
        item->setData(Qt::UserRole, it.id);
        ui->lwModul->addItem(item);
    }

    Products.FindItems("", Status::FAULTY);
    // repo.LoadProducts(listProduct, Status::FAULTY);
    ui->lwProduct->clear();
    for(auto &it : Products.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it.name + " №" + it.numAndComment());
        item->setData(Qt::UserRole, it.id);
        ui->lwProduct->addItem(item);
    }

}


void OTKControlWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
        if(!scanNumber.isEmpty())
        {

        }

        scanNumber.clear();
    }
    else
    {
        scanNumber += event->text();
    }

}


//---------------------------------------------------------------------------------------
// Кнопка Модуль Проверку прошел
//---------------------------------------------------------------------------------------
// void OTKControlWindow::on_pbCheck_clicked()
// {
//     QListWidgetItem *item = ui->lwModul->currentItem();
//     if(item == nullptr)
//         return;
//     QListWidgetItem *item2 = new QListWidgetItem(*item);
//     int idModul = item->data(Qt::UserRole).toInt();

//     Status status;
//     status.idDevice = idModul;
//     status.idStatus = Status::CORRECT;
//     status.dateStatus = QDateTime::currentDateTime();

//     listStatus[idModul] = status;;

//     ui->lwChecked->addItem(item2);
//     delete item;
// }


//---------------------------------------------------------------------------------------
// Кнопка Модуль Проверку не прошел
//---------------------------------------------------------------------------------------
// void OTKControlWindow::on_pbBroken_clicked()
// {
//     QListWidgetItem *item = ui->lwModul->currentItem();
//     if(item == nullptr)
//         return;

//     QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");

//     QListWidgetItem *item2 = new QListWidgetItem(*item);
//     int idModul = item->data(Qt::UserRole).toInt();
//     auto modul = std::find_if(Modules.listItems.cbegin(), Modules.listItems.cend(), [&] (const Modul p) { return p.id == idModul;});
//     Modul mod = *modul;

//     Status status;
//     status.idDevice = idModul;
//     status.idStatus = Status::FAULTY;
//     status.Comment = comment;
//     status.dateStatus = QDateTime::currentDateTime();
//     listStatus[idModul] = status;;
//     mod.listStatus.push_back(status);
//     item2->setText(mod.numAndComment());

//     ui->lwBroken->addItem(item2);
//     delete item;
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
    item2->setText(mod.numAndComment());
    ui->lwModul->addItem(item2);

    listStatus.remove(idModul);
    delete item;

}


//---------------------------------------------------------------------------------------
// Кнопка Изелие Проверку прошел
//---------------------------------------------------------------------------------------
// void OTKControlWindow::on_pbCheckProd_clicked()
// {
//     QListWidgetItem *item = ui->lwProduct->currentItem();
//     if(item == nullptr)
//         return;
//     QListWidgetItem *item2 = new QListWidgetItem(*item);
//     int idProd = item->data(Qt::UserRole).toInt();

//     Status status;
//     status.idDevice = idProd;
//     status.idStatus = Status::CORRECT;
//     status.dateStatus = QDateTime::currentDateTime();

//     listStatusProd[idProd] = status;;

//     ui->lwCheckedProd->addItem(item2);
//     delete item;
// }


//---------------------------------------------------------------------------------------
// Кнопка Изелие Проверку не прошел
//---------------------------------------------------------------------------------------
// void OTKControlWindow::on_pbBrokenProd_clicked()
// {
//     QListWidgetItem *item = ui->lwProduct->currentItem();
//     if(item == nullptr)
//         return;

//     QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");

//     QListWidgetItem *item2 = new QListWidgetItem(*item);

//     int idProd = item->data(Qt::UserRole).toInt();
//     auto prod = std::find_if(Products.listItems.cbegin(), Products.listItems.cend(), [&] (const Product p) { return p.id == idProd;});
//     Product product = *prod;

//     Status status;
//     status.idDevice = idProd;
//     status.Comment = comment;
//     status.idStatus = Status::FAULTY;
//     status.dateStatus = QDateTime::currentDateTime();
//     listStatusProd[idProd] = status;;
//     product.listStatus.push_back(status);
//     item2->setText(product.numAndComment());
//     ui->lwBrokenProd->addItem(item2);
//     delete item;
// }


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
    item2->setText( product.numAndComment());
    ui->lwProduct->addItem(item2);

    listStatusProd.remove(idProd);
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

    if(ui->tabWidget->currentIndex() == 0)
    {
        for(int row = 0; row < ui->lwProduct->count(); ++row)
        {
            QListWidgetItem *item = ui->lwProduct->item(row);
            if(item->data(Qt::UserRole + 1).toString() == s)
            {
                ItemCheckedControl(item);

                // status.idDevice = item->data(Qt::UserRole).toInt();
                // listStatusProd[status.idDevice] = status;
                // QListWidgetItem *item2 = new QListWidgetItem(*item);
                // ui->lwCheckedProd->addItem(item2);
                // delete item;
            }
        }
    }
    else
    {
        for(int row = 0; row < ui->lwModul->count(); ++row)
        {
            QListWidgetItem *item = ui->lwModul->item(row);
            if(item->data(Qt::UserRole + 1).toString() == s)
            {
                ItemCheckedControl(item);
                // status.idDevice = item->data(Qt::UserRole).toInt();
                // listStatus[status.idDevice] = status;
                // QListWidgetItem *item2 = new QListWidgetItem(*item);
                // ui->lwChecked->addItem(item2);
                // delete item;
            }
        }
    }

}


//---------------------------------------------------------------------------------------
// Добавление выбранного элемента из списка изделий или модулей
//---------------------------------------------------------------------------------------
void OTKControlWindow::ItemCheckedControl(QListWidgetItem *item)
{
    if(item == nullptr)
        return;

    if(ui->tabWidget->currentIndex() == 0)
    {
        QListWidgetItem *item2 = new QListWidgetItem(*item);
        int idProd = item->data(Qt::UserRole).toInt();
        Product product = Products.GetItem(idProd);

        Status status;
        status.idDevice = idProd;
        status.dateStatus = QDateTime::currentDateTime();

        if(ui->rbCheck->isChecked())
        {
            status.idStatus = Status::CORRECT;
            ui->lwCheckedProd->addItem(item2);
        }
        else
        {
            QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");
            status.Comment = comment;
            status.idStatus = Status::FAULTY;

            item2->setText(product.numAndComment());
            ui->lwBrokenProd->addItem(item2);
        }

        product.listStatus.push_back(status);
        listStatusProd[idProd] = status;
        delete item;
    }
    else
    {
        QListWidgetItem *item2 = new QListWidgetItem(*item);
        int idModul = item->data(Qt::UserRole).toInt();
        Modul mod = Modules.GetItem(idModul);

        Status status;
        status.idDevice = idModul;
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
            item2->setText(mod.numAndComment());
            ui->lwBroken->addItem(item2);
        }
        listStatus[idModul] = status;;
        mod.listStatus.push_back(status);
        delete item;
    }

}



//---------------------------------------------------------------------------------------
// Кнопка Проверено
//---------------------------------------------------------------------------------------
void OTKControlWindow::on_pbChecked_clicked()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        QListWidgetItem *item = ui->lwProduct->currentItem();
        ItemCheckedControl(item);
        // if(item == nullptr)
        //     return;

        // QListWidgetItem *item2 = new QListWidgetItem(*item);
        // int idProd = item->data(Qt::UserRole).toInt();
        // Product product = Products.GetItem(idProd);

        // Status status;
        // status.idDevice = idProd;
        // status.dateStatus = QDateTime::currentDateTime();

        // if(ui->rbCheck->isChecked())
        // {
        //     status.idStatus = Status::CORRECT;
        //     ui->lwCheckedProd->addItem(item2);
        // }
        // else
        // {
        //     QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");
        //     status.Comment = comment;
        //     status.idStatus = Status::FAULTY;

        //     item2->setText(product.numAndComment());
        //     ui->lwBrokenProd->addItem(item2);
        // }

        // product.listStatus.push_back(status);
        // listStatusProd[idProd] = status;
        // delete item;
    }
    else
    {
        QListWidgetItem *item = ui->lwModul->currentItem();
        // if(item == nullptr)
        //     return;
        ItemCheckedControl(item);


        // QListWidgetItem *item2 = new QListWidgetItem(*item);
        // int idModul = item->data(Qt::UserRole).toInt();
        // Modul mod = Modules.GetItem(idModul);

        // Status status;
        // status.idDevice = idModul;
        // status.dateStatus = QDateTime::currentDateTime();

        // if(ui->rbCheck->isChecked())
        // {
        //     status.idStatus = Status::CORRECT;
        //     ui->lwChecked->addItem(item2);
        // }
        // else
        // {
        //     QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");
        //     status.idStatus = Status::FAULTY;
        //     status.Comment = comment;
        //     item2->setText(mod.numAndComment());
        //     ui->lwBroken->addItem(item2);
        // }
        // listStatus[idModul] = status;;
        // mod.listStatus.push_back(status);
        // delete item;
    }
}

