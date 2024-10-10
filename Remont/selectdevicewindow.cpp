#include "selectdevicewindow.h"
#include "ui_selectdevicewindow.h"



SelectDeviceWindow::SelectDeviceWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::SelectDeviceWindow)
{
    ui->setupUi(this);
}

SelectDeviceWindow::~SelectDeviceWindow()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------
// Поиск устройства по списку статусов
//--------------------------------------------------------------------------------------------------
IDevice* SelectDeviceWindow::SelectDevice(bool isNow, QString searchNum, QVector<Status::Stat> statusList)
{
    listStatus = statusList;
    ui->leSearch->setText(searchNum);

    // есди , то сразу поиск
    if(isNow)
    {
        Search(searchNum);
        if(listProduct.listItems.size() + listModul.listItems.size() == 1 && !searchNum.isEmpty())
        {
            // если найден единственный экземпляр, то выходим
            if(listProduct.listItems.size() == 1)
            {
                device = new Product;
                *device = listProduct.listItems.first();
                // return listProduct.listItems.first();
            }
            else
            {
                device = new Modul;
                *device = listModul.listItems.first();

                // return listModul.listItems.first();
            }
            // accept();
            return device;
        }
        else
        {
            // если много, то в список и на экран
            listToScreen();
        }
    }

    exec();
    // if(exec() == QDialog::Accepted)
    return device;
}


//--------------------------------------------------------------------------------------------------
// Поиск устройства по статусу
//--------------------------------------------------------------------------------------------------
IDevice* SelectDeviceWindow::SelectDevice(bool isNow, QString searchNum, Status::Stat status)
{
    QVector<Status::Stat> statusList = {status};
    return SelectDevice(isNow, searchNum, statusList);
}

//--------------------------------------------------------------------------------------------------
// Установка типа устройств
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::setTypeSearch(TypeDevice type)
{
    typeDevice = type;
    // скрывем ненужную закладку
    if(typeDevice == TypeProduct)
        ui->tabWidget->setTabVisible(1, false);

    if(typeDevice == TypeModul)
        ui->tabWidget->setTabVisible(0, false);

}

void SelectDeviceWindow::setNotShipped()
{
    isNotShipped = false;
}

//--------------------------------------------------------------------------------------------------
// Установка флага немедленного поиска
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::setDisaleSearch()
{
    ui->leSearch->setEnabled(false);
    ui->pbSelect->setEnabled(false);
}


//--------------------------------------------------------------------------------------------------
// Искать по номеру и установленным параметрам
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::Search(QString number)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    listProduct.listItems.clear();
    listModul.listItems.clear();
    for(auto &it : listStatus)
    {
        if(typeDevice != TypeDevice::TypeModul)
        {
            QList<Product> products;
            repo.FindItems(number, products, it, isNotShipped);
            listProduct.listItems += products;
        }

        if(typeDevice != TypeDevice::TypeProduct)
        {
            QList<Modul> moduls;
            repo.FindItems(number, moduls, it, isNotShipped);
            listModul.listItems += moduls;
        }
    }

    QApplication::restoreOverrideCursor();

}


//--------------------------------------------------------------------------------------------------
// Вывод списков на экран
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::listToScreen()
{
    int row = 0;

    ui->twProduct->setRowCount(listProduct.listItems.size());

    for(auto const &it : listProduct.listItems)
    {
        // ui->twProduct->insertRow(row);
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.number);
        // QVariant var;
        // var.setValue(&it);
        item->setData(Qt::UserRole, it.id);
        ui->twProduct->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.name);
        ui->twProduct->setItem(row, 1, item);

        if(it.listStatus.size() > 0)
        {
            item = new QTableWidgetItem();
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setText(it.getNameLastStatus());
            ui->twProduct->setItem(row, 2, item);
        }

        ++row;
    }
    ui->twProduct->resizeColumnsToContents();
    ui->twProduct->resizeRowsToContents();

    row = 0;
    ui->twModul->setRowCount(listModul.listItems.size());
    for(auto const &it : listModul.listItems)
    {
        // ui->twModul->insertRow(row);
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.number);
        item->setData(Qt::UserRole, it.id);
        ui->twModul->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.name);
        ui->twModul->setItem(row, 1, item);

        if(it.listStatus.size() > 0)
        {
            item = new QTableWidgetItem();
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setText(it.listStatus.last().nameStatus);
            ui->twModul->setItem(row, 2, item);
        }
        ++row;

    }
    ui->twModul->resizeColumnsToContents();
    ui->twModul->resizeRowsToContents();

}


//--------------------------------------------------------------------------------------------------
// Кнопка Искать
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::on_tbSearch_clicked()
{
    // if(ui->leSearch->text().isEmpty())
    //     return;
    Search(ui->leSearch->text());
    listToScreen();

    // return;

    // QApplication::setOverrideCursor(Qt::WaitCursor);

    // int resMod = 0;
    // int resProd = 0;

    // if(typeDevice == TypeDevice::TypeModul || typeDevice == TypeDevice::TypeAll)
    // {
    //     resMod = SearchModul(ui->leSearch->text(), status);
    // }

    // if(typeDevice == TypeDevice::TypeProduct || typeDevice == TypeDevice::TypeAll)
    // {
    //     resProd += SearchProduct(ui->leSearch->text(), status);
    // }

    // if(resProd + resMod == 1)
    // {
    //     setGeometry(5000,5000,0,0);
    //     startTimer(10);
    //     // QApplication::postEvent(this, )
    //     if(listModul.listItems.size() > 0)
    //         modul = listModul.listItems.at(0);
    //     if(listProduct.listItems.size() > 0)
    //         prod = listProduct.listItems.at(0);

    // }

    // QApplication::restoreOverrideCursor();
}


//--------------------------------------------------------------------------------------------------
// Кнопка Выбрать
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::on_pbSelect_clicked()
{

    if(ui->tabWidget->currentIndex() == 0 && ui->twProduct->currentRow() >= 0)
    {
        device = new Product;
        *device = listProduct.listItems[ui->twProduct->currentRow()];
    }

    if(ui->tabWidget->currentIndex() == 1 && ui->twModul->currentRow() >= 0)
    {
        device = new Modul;
        *device = listModul.listItems[ui->twModul->currentRow()];
    }

    accept();
}


void SelectDeviceWindow::on_twModul_cellDoubleClicked(int /*row*/, int /*column*/)
{
    on_pbSelect_clicked();
}


void SelectDeviceWindow::on_twProduct_cellDoubleClicked(int /*row*/, int /*column*/)
{
    on_pbSelect_clicked();
}


//--------------------------------------------------------------------------------------------------
// Поиск модуля по номеру
//--------------------------------------------------------------------------------------------------

// int SelectDeviceWindow::SearchModul(QString number, int status)
// {
//     ui->twModul->setRowCount(0);

//     // if(number.isEmpty())
//     //     listModul.FindItems("", status);
//     // else
//         listModul.FindItems(number, status);

//     int row = 0;
//     for(auto const &it : listModul.listItems)
//     {
//         ui->twModul->insertRow(row);
//         QTableWidgetItem *item = new QTableWidgetItem();
//         item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//         item->setText(it.number);
//         item->setData(Qt::UserRole, it.id);
//         ui->twModul->setItem(row, 0, item);

//         item = new QTableWidgetItem();
//         item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//         item->setText(it.name);
//         ui->twModul->setItem(row, 1, item);

//         if(it.listStatus.size() > 0)
//         {
//             item = new QTableWidgetItem();
//             item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//             item->setText(it.listStatus.last().nameStatus);
//             ui->twModul->setItem(row, 2, item);
//         }
//         ++row;

//     }
//     ui->twModul->resizeColumnsToContents();
//     ui->twModul->resizeRowsToContents();

//     if(listModul.listItems.size() == 0)
//         ui->tabWidget->setCurrentIndex(0);

//     return row;
// }

//--------------------------------------------------------------------------------------------------
// Поиск изделия по номеру
//--------------------------------------------------------------------------------------------------
// int SelectDeviceWindow::SearchProduct(QString number, int status)
// {
//     ui->twProduct->setRowCount(0);

//     // if(number.isEmpty())
//     //     listProduct.FindItems("", status);
//     // else
//         listProduct.FindItems(number, status);


//     int row = 0;
//     for(auto const &it : listProduct.listItems)
//     {
//         ui->twProduct->insertRow(row);
//         QTableWidgetItem *item = new QTableWidgetItem();
//         item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//         item->setText(it.number);
//         // QVariant var;
//         // var.setValue(&it);
//         item->setData(Qt::UserRole, it.id);
//         ui->twProduct->setItem(row, 0, item);

//         item = new QTableWidgetItem();
//         item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//         item->setText(it.name);
//         ui->twProduct->setItem(row, 1, item);

//         if(it.listStatus.size() > 0)
//         {
//             item = new QTableWidgetItem();
//             item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//             item->setText(it.getNameLastStatus());
//             ui->twProduct->setItem(row, 2, item);
//         }

//         ++row;
//     }
//     ui->twProduct->resizeColumnsToContents();
//     ui->twProduct->resizeRowsToContents();

//     if(listProduct.listItems.size() == 0)
//         ui->tabWidget->setCurrentIndex(1);

//     return row;
// }

// void SelectDeviceWindow::timerEvent(QTimerEvent *event)
// {
//     killTimer(event->timerId());
//     accept();
// }

