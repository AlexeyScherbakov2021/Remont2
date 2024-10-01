#include "listshipwindow.h"
// #include "shipwindow.h"
#include "shipwindow.h"
#include "ui_listshipwindow.h"

ListShipWindow::ListShipWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListShipWindow)
{
    ui->setupUi(this);

    QApplication::changeOverrideCursor(Qt::WaitCursor);

    Ship.FindItems();

    // repo.LoadShipment(listShip);

    ui->tableWidget->setRowCount(Ship.listItems.size());

    int row = 0;
    for(auto &it : Ship.listItems)
    {

        QTableWidgetItem *item = new QTableWidgetItem(it.number);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 0, item);

        item = new QTableWidgetItem(it.schet);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 1, item);

        item = new QTableWidgetItem(it.customer);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 2, item);

        item = new QTableWidgetItem(it.cardOrder);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 3, item);

        item = new QTableWidgetItem(it.objectInstall);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 4, item);

        ++row;
    }

    // ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    QApplication::restoreOverrideCursor();

}

ListShipWindow::~ListShipWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------
// Кнопка Добавить новую отгрузку
//----------------------------------------------------------------------------
void ListShipWindow::on_pbNew_clicked()
{
    Shipment ship;
    ShipWindow *win = new ShipWindow(&ship, this);
    if(win->exec() == QDialog::Accepted)
    {
        Ship.listItems.removeIf([ship](Shipment s) { return s.id == ship.id; });
    }
    else
        Ship.listItems.push_back(ship);
}


//----------------------------------------------------------------------------
// Кнопка Редактировать
//----------------------------------------------------------------------------
void ListShipWindow::on_pbEdit_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row < 0)
        return;

    on_tableWidget_cellDoubleClicked(row, 0);
}


//----------------------------------------------------------------------------
// Двойной щелчок в таблице
//----------------------------------------------------------------------------
void ListShipWindow::on_tableWidget_cellDoubleClicked(int row, int /*column*/)
{
    Shipment ship = Ship.listItems[row];

    ShipWindow *win = new ShipWindow(&ship, this);
    if(win->exec() == QDialog::Accepted)
    {
        Ship.listItems.removeIf([ship](Shipment s) { return s.id == ship.id; });
    }
    else
        Ship.listItems[row] = ship;
}


//----------------------------------------------------------------------------
// Кнопка Удалить отгрузку
//----------------------------------------------------------------------------
void ListShipWindow::on_pbDeleteShip_clicked()
{

}

