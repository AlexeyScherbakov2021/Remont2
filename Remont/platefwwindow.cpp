#include "platefwwindow.h"
#include "platelistwindow.h"
#include "ui_platefwwindow.h"

PlateFWWindow::PlateFWWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateFWWindow)
{
    ui->setupUi(this);
}

PlateFWWindow::~PlateFWWindow()
{
    delete ui;
}

void PlateFWWindow::on_tbSearch_clicked()
{
    PlateListWindow *selPlate = new PlateListWindow(this);
    selPlate->RemoveListPlate(listPlate.listItems);
    selPlate->setSelect();
    Plate plate = selPlate->SelectPlate(ui->leSearch->text());

    if(plate.id == 0)
        return;

    listPlate.listItems.append(plate);

    int row = ui->twListPlate->rowCount();
    ui->twListPlate->insertRow(row);

    QTableWidgetItem *item = new QTableWidgetItem(plate.number);
    item->setData(Qt::UserRole, plate.id);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twListPlate->setItem(row, 0 , item);

    item = new QTableWidgetItem(plate.number2);
    item->setData(Qt::UserRole, plate.id);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twListPlate->setItem(row, 1 , item);

    ui->twListPlate->resizeColumnsToContents();
    ui->twListPlate->resizeRowsToContents();
}


void PlateFWWindow::on_pbApply_clicked()
{
    QString numberFW = ui->leFW->text();
    if(numberFW.isEmpty())
        return;

    for(int i = 0; i < listPlate.listItems.size(); ++i)
    {
        listPlate.listItems[i].number2 = numberFW;
        ui->twListPlate->item(i, 1)->setText(numberFW);
    }
}


void PlateFWWindow::on_pbOK_clicked()
{
    for(auto it : listPlate.listItems)
        listPlate.UpdateItem(it);

    accept();
}


void PlateFWWindow::on_tbDelete_clicked()
{
    int row = ui->twListPlate->currentRow();
    if(row < 0)
        return;

    int id = ui->twListPlate->item(row, 0)->data(Qt::UserRole).toInt();
    listPlate.listItems.removeIf( [id] (const Plate &plate) { return plate.id == id; } );

    ui->twListPlate->removeRow(row);
}

