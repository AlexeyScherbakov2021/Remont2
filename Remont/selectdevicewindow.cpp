#include "selectdevicewindow.h"
#include "ui_selectdevicewindow.h"

SelectDeviceWindow::SelectDeviceWindow(QWidget *parent, QString searchNum, Status::Stat status, TypeDevice typeDevice)
    : QDialog(parent)
    , ui(new Ui::SelectDeviceWindow), status(status), typeDevice(typeDevice)
{
    ui->setupUi(this);

    if(typeDevice == TypeProduct)
        ui->tabWidget->setTabVisible(1, false);

    if(typeDevice == TypeModul)
        ui->tabWidget->setTabVisible(0, false);

    if(!searchNum.isEmpty())
    {
        ui->leSearch->setText(searchNum);
        ui->leSearch->setEnabled(false);
        on_tbSearch_clicked();
    }

}

SelectDeviceWindow::~SelectDeviceWindow()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------
// Кнопка Искать
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::on_tbSearch_clicked()
{
    // if(ui->leSearch->text().isEmpty())
    //     return;

    int resMod = 0;
    int resProd = 0;

    if(typeDevice == TypeDevice::TypeModul || typeDevice == TypeDevice::TypeAll)
        resMod = SearchModul(ui->leSearch->text(), status);

    if(typeDevice == TypeDevice::TypeProduct || typeDevice == TypeDevice::TypeAll)
        resProd = SearchProduct(ui->leSearch->text(), status);

    if(resProd + resMod == 1)
    {
        setGeometry(5000,5000,0,0);
        startTimer(10);
        // QApplication::postEvent(this, )
        if(listModul.listItems.size() > 0)
            modul = listModul.listItems.at(0);
        if(listProduct.listItems.size() > 0)
            prod = listProduct.listItems.at(0);

    }
}


//--------------------------------------------------------------------------------------------------
// Кнопка Выбрать
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::on_pbSelect_clicked()
{

    if(ui->tabWidget->currentIndex() == 0 && ui->twProduct->currentRow() >= 0)
        prod = listProduct.listItems[ui->twProduct->currentRow()];

    if(ui->tabWidget->currentIndex() == 1 && ui->twModul->currentRow() >= 0)
        modul = listModul.listItems[ui->twModul->currentRow()];

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
int SelectDeviceWindow::SearchModul(QString number, int status)
{

    ui->twModul->setRowCount(0);

    if(number.isEmpty())
        listModul.FindItems(status);
    else
        listModul.FindItems(number, status);

    int row = 0;
    for(auto const &it : listModul.listItems)
    {
        ui->twModul->insertRow(row);
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

    if(listModul.listItems.size() == 0)
        ui->tabWidget->setCurrentIndex(0);

    return row;
}

//--------------------------------------------------------------------------------------------------
// Поиск изделия по номеру
//--------------------------------------------------------------------------------------------------
int SelectDeviceWindow::SearchProduct(QString number, int status)
{
    ui->twProduct->setRowCount(0);

    if(number.isEmpty())
        listProduct.FindItems(status);
    else
        listProduct.FindItems(number, status);

    int row = 0;
    for(auto const &it : listProduct.listItems)
    {
        ui->twProduct->insertRow(row);
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

    if(listProduct.listItems.size() == 0)
        ui->tabWidget->setCurrentIndex(1);

    return row;
}

void SelectDeviceWindow::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());
    accept();
}

