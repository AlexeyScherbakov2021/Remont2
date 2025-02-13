#include "platefwwindow.h"
#include "platelistwindow.h"
#include "scan.h"
#include "ui_platefwwindow.h"



PlateFWWindow::PlateFWWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateFWWindow)
{
    ui->setupUi(this);

    model = new DeviceModel(ItemType::Plate, this);
    model->setBaseOff();
    // model->setFunction((PlateModel::pLoadItems)&PlateFWWindow::LoadItems);
    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 20);
    ui->tableView->setColumnWidth(6, 150);


    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));
}

PlateFWWindow::~PlateFWWindow()
{
    disconnect(conn);
    delete ui;
}

// void PlateFWWindow::LoadItems(QList<Items> &items)
// {

// }

void PlateFWWindow::on_tbSearch_clicked()
{
    PlateListWindow *selPlate = new PlateListWindow(this);
    // selPlate->RemoveListPlate(listPlate.items);
    selPlate->setSelectForm();
    selPlate->SelectPlate(ui->leSearch->text());

    // selPlate->exec();

    for(auto &it : selPlate->selectedPlates)
    {
        model->AddItem(&it);
    //     listPlate.listItems.push_back(it);

    //     int row = ui->twListPlate->rowCount();
    //     ui->twListPlate->insertRow(row);

    //     QTableWidgetItem *item = new QTableWidgetItem(it.number);
    //     item->setData(Qt::UserRole, it.id);
    //     item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    //     ui->twListPlate->setItem(row, 0 , item);

    //     item = new QTableWidgetItem(it.number2);
    //     item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    //     ui->twListPlate->setItem(row, 1 , item);

    //     ui->twListPlate->resizeColumnsToContents();
    //     ui->twListPlate->resizeRowsToContents();
    }
}


void PlateFWWindow::on_pbApply_clicked()
{
    QString numberFW = ui->leFW->text();
    if(numberFW.isEmpty())
        return;

    for(int row = 0; row < model->rowCount(); ++row)
    {
        model->setData(model->index(row, 1), numberFW);
    }

}


void PlateFWWindow::on_pbOK_clicked()
{
    for(int row = 0; row < model->rowCount(); ++row)
    {
        model->UpdateItem(row);
    }


    // for(auto it : listPlate.items)
    //     listPlate.UpdateItem(it);

    accept();
}


void PlateFWWindow::on_tbDelete_clicked()
{
    // auto ranges = ui->twListPlate->selectedRanges();

    // for(int i = ranges.size() - 1; i >= 0; --i)
    // {
    //     for(int row = ranges[i].bottomRow(); row >= ranges[i].topRow(); row--)
    //     {
    //         int id = ui->twListPlate->item(row, 0)->data(Qt::UserRole).toInt();
    //         listPlate.items.removeIf( [id] (const Items &plate) { return plate.id == id; } );
    //         ui->twListPlate->removeRow(row);
    //     }
    // }
}

void PlateFWWindow::slotReadScan(QString /*s*/)
{
    if(!isActiveWindow())
        return;

    // PlateListWindow *selPlate = new PlateListWindow(this);
    // selPlate->RemoveListPlate(listPlate.listItems);
    // selPlate->setSelect();
    // Plate plate = selPlate->SelectPlate(s);
    // if(plate.id <= 0)
    //     return;


    // listPlate.listItems.push_back(plate);

    // int row = ui->twListPlate->rowCount();
    // ui->twListPlate->insertRow(row);

    // QTableWidgetItem *item = new QTableWidgetItem(plate.number);
    // item->setData(Qt::UserRole, plate.id);
    // item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    // ui->twListPlate->setItem(row, 0 , item);

    // item = new QTableWidgetItem(plate.number2);
    // item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    // ui->twListPlate->setItem(row, 1 , item);

    // ui->twListPlate->resizeColumnsToContents();
    // ui->twListPlate->resizeRowsToContents();


}

