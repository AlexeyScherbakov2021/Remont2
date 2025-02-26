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

    ui->pbNew->setVisible(!_isShip);
    ui->pbEdit->setVisible(!_isShip);
    ui->pbDeleteShip->setVisible(!_isShip);

    model = new ShipModel(this);
    model->prepareLoad("", isShip);

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(3, 310);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 80);

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
    QScopedPointer<ShipWindow> win(new ShipWindow(&ship));
    int res = win->exec();
    if(res == QDialog::Accepted)
    {
        model->AddItemToList(&ship);
    }
}


//----------------------------------------------------------------------------
// Кнопка Удалить отгрузку
//----------------------------------------------------------------------------
void ListShipWindow::on_pbDeleteShip_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if( row < 0)
        return;

    Shipment *ship = model->GetItem(row);

    if(QMessageBox::question(this, "Предупреждение",
            QString("Удалить отгрузку счет \"%1\"?").arg(ship->schet), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        model->DeleteItem(row);

        // if(repo.DeleteShipment(ship.id))
        // {
        //     ui->tableWidget->removeRow(row);
        // }
    }
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

