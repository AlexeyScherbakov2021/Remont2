#include "listshipwindow.h"
// #include "shipwindow.h"
#include "shipwindow.h"
#include "ui_listshipwindow.h"

#include <QElapsedTimer>
#include <QMessageBox>

ListShipWindow::ListShipWindow(QWidget *parent, bool _isShip)
    : QDialog(parent)
    , ui(new Ui::ListShipWindow), isShip(_isShip)
{
    ui->setupUi(this);

    QApplication::changeOverrideCursor(Qt::WaitCursor);

    model = new ShipModel(this);

    model->prepareLoad("", isShip);
    ui->tableView->setModel(model);
    // QElapsedTimer t;
    // t.start();
    // qDebug() << t.elapsed() << "Start select";

    // Ship.FindItems("", 0, isFree);

    // qDebug() << t.elapsed() << "End select";
    // t.restart();

    // ui->tableWidget->setRowCount(Ship.listItems.size());
    // ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 200);
    // ui->tableView->setColumnWidth(2, 80);
    ui->tableView->setColumnWidth(3, 310);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 80);

    // int row = 0;
    // for(auto &it : Ship.listItems)
    // {
    //     addRowWidget(it, row);
    //     ++row;
    // }

    // qDebug() << t.elapsed() << "Added to Table";
    // t.restart();

    // ui->tableWidget->resizeColumnsToContents();
    // ui->tableWidget->resizeRowsToContents();

    // qDebug() << t.elapsed() << "Resize Table";

    QApplication::restoreOverrideCursor();



}

ListShipWindow::~ListShipWindow()
{
    delete ui;
}

// void ListShipWindow::addRowWidget(Shipment &ship, int row)
// {
//     if(row == -1)
//     {
//         row = ui->tableWidget->rowCount();
//         ui->tableWidget->insertRow(row);
//     }

//     QTableWidgetItem *item = new QTableWidgetItem(ship.schet);
//     item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//     ui->tableWidget->setItem(row, 0, item);

//     item = new QTableWidgetItem(ship.customer);
//     item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//     ui->tableWidget->setItem(row, 1, item);

//     item = new QTableWidgetItem(ship.cardOrder);
//     item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//     ui->tableWidget->setItem(row, 2, item);

//     item = new QTableWidgetItem(ship.objectInstall);
//     item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//     ui->tableWidget->setItem(row, 3, item);

//     item = new QTableWidgetItem(ship.numberUPD);
//     item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//     ui->tableWidget->setItem(row, 4, item);

//     item = new QTableWidgetItem(ship.dateUPD.toString("dd.MM.yyyy"));
//     item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//     ui->tableWidget->setItem(row, 5, item);
// }

// void ListShipWindow::setRowWidget(Shipment &ship, int row)
// {
//     QTableWidgetItem *item = ui->tableWidget->item(row, 0);
//     item->setText(ship.schet);

//     item = ui->tableWidget->item(row, 1);
//     item->setText(ship.customer);

//     item = ui->tableWidget->item(row, 2);
//     item->setText(ship.cardOrder);

//     item = ui->tableWidget->item(row, 3);
//     item->setText(ship.objectInstall);

//     // item = ui->tableWidget->item(row, 4);
//     // item->setText(ship.objectInstall);
// }

//----------------------------------------------------------------------------
// Кнопка Добавить новую отгрузку
//----------------------------------------------------------------------------
void ListShipWindow::on_pbNew_clicked()
{
    // Shipment ship;
    // ShipWindow *win = new ShipWindow(&ship, this);
    // int res = win->exec();
    // qDebug() << res;
    // if(res == QDialog::Accepted)
    // {
    //     if(!ship.dateRegister.isValid())
    //     {
    //         Ship.listItems.push_back(ship);
    //         addRowWidget(ship);
    //     }
    // }
}




//----------------------------------------------------------------------------
// Двойной щелчок в таблице
//----------------------------------------------------------------------------
// void ListShipWindow::on_tableWidget_cellDoubleClicked(int row, int /*column*/)
// {
//     Shipment ship = Ship.listItems[row];

//     ShipWindow *win = new ShipWindow(&ship, this);
//     if(win->exec() == QDialog::Accepted)
//     {
//         if(ship.dateRegister.isValid())
//         {
//             Ship.listItems.removeIf([ship](Shipment s) { return s.id == ship.id; });
//             ui->tableWidget->removeRow(row);
//         }
//         else
//         {
//             setRowWidget(ship, row);
//             Ship.listItems[row] = ship;
//         }
//     }
// }


//----------------------------------------------------------------------------
// Кнопка Удалить отгрузку
//----------------------------------------------------------------------------
void ListShipWindow::on_pbDeleteShip_clicked()
{
    // int row = ui->tableWidget->currentRow();
    // if( row < 0)
    //     return;

    // Shipment ship = Ship.listItems[row];

    // if(QMessageBox::question(this, "Предупреждение",
    //         QString("Удалить отгрузку счет \"%1\"?").arg(ship.schet), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    // {
    //     if(repo.DeleteShipment(ship.id))
    //     {
    //         ui->tableWidget->removeRow(row);
    //     }
    // }
}


//----------------------------------------------------------------------------
// Кнопка Редактировать
//----------------------------------------------------------------------------
void ListShipWindow::on_pbEdit_clicked()
{
    // int row = ui->tableWidget->currentRow();
    // if(row < 0)
    //     return;

    // on_tableWidget_cellDoubleClicked(row, 0);

    on_tableView_doubleClicked(ui->tableView->currentIndex());
}


//----------------------------------------------------------------------------
// Двойной щелчок в таблице
//----------------------------------------------------------------------------
void ListShipWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    Shipment *ship = model->GetItem(index.row());
    if(ship == nullptr)
        return;

    QScopedPointer<ShipWindow> win(new ShipWindow(ship));
    if(win->exec() == QDialog::Accepted)
    {
        if(ship->dateUPD.date().year() > 1900)
        {
            model->DeleteItemFromList(index.row());
        }
        // qDebug() << "Измение в строке" << ship->schet << ship->cardOrder;
    }
}


void ListShipWindow::on_tbSearch_clicked()
{
    model->prepareLoad(ui->leSearch->text(), isShip);
}

