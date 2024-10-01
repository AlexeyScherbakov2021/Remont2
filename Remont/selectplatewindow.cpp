#include "selectplatewindow.h"
#include "ui_selectplatewindow.h"

SelectPlateWindow::SelectPlateWindow(QList<Plate> &listPlate, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectPlateWindow), selectedIndex(-1)
{
    ui->setupUi(this);

    int row = 0;
    for(auto &it : listPlate)
    {
        ui->tableWidget->insertRow(row);

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.dateRegister.toString("dd.MM.yyyy"));
        item->setData(Qt::UserRole, row);
        ui->tableWidget->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.number);
        ui->tableWidget->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.number2);
        ui->tableWidget->setItem(row, 2, item);

        item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.VNFT);
        ui->tableWidget->setItem(row, 3, item);

        item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText(it.numberDoc);
        ui->tableWidget->setItem(row, 4, item);

        ++row;
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setCurrentCell(0, 0);
}

SelectPlateWindow::~SelectPlateWindow()
{
    delete ui;
}

void SelectPlateWindow::on_SelectPlateWindow_accepted()
{
    selectedIndex = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt();
}


void SelectPlateWindow::on_tableWidget_cellDoubleClicked(int /*row*/, int /*column*/)
{
    accept();
}

