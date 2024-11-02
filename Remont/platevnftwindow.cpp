#include "platevnftwindow.h"
#include "ui_platevnftwindow.h"

#include <models/platetype.h>

#include <qmessagebox.h>

PlateVNFTWindow::PlateVNFTWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateVNFTWindow)
{
    ui->setupUi(this);

    model2.setTable("PlateType");
    model2.select();
    model2.setEditStrategy(QSqlTableModel::OnManualSubmit);
    model2.setHeaderData(1, Qt::Horizontal, "Децимальный номер");
    model2.setHeaderData(2, Qt::Horizontal, "Наименование");

    ui->tableView->setModel(&model2);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->setAlternatingRowColors(true);
}

PlateVNFTWindow::~PlateVNFTWindow()
{
    delete ui;
}

//------------------------------------------------------------------------------------------------
// Изменение ячейки
//------------------------------------------------------------------------------------------------
// void PlateVNFTWindow::on_twVNFT_itemChanged(QTableWidgetItem *item)
// {

// }


//------------------------------------------------------------------------------------------------
// Добавление строки
//------------------------------------------------------------------------------------------------
void PlateVNFTWindow::on_tbAdd_clicked()
{
    int row = model2.rowCount();
    model2.insertRow(row);
    ui->tableView->selectRow(row);
    ui->tableView->edit(model2.index(row, 1));
    ui->tableView->resizeRowToContents(row);
}


//------------------------------------------------------------------------------------------------
// Удаление строки
//------------------------------------------------------------------------------------------------
void PlateVNFTWindow::on_tbDelete_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    model2.removeRow(index.row());
}

void PlateVNFTWindow::on_pbSave_clicked()
{
    bool res = model2.submitAll();
    if(!res)
        QMessageBox::critical(this, "Ошибка", "Произошла ошибка при сохранении базы данных.");
}


void PlateVNFTWindow::on_pbCancel_clicked()
{
    model2.revertAll();
}

