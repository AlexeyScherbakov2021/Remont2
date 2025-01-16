#include "prodvnftwindow.h"
#include "ui_prodvnftwindow.h"

#include <models/prodtype.h>

#include <qmessagebox.h>

ProdVNFTWindow::ProdVNFTWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProdVNFTWindow)
{
    ui->setupUi(this);

    model2.setTable("ProductType");
    model2.select();
    model2.setEditStrategy(QSqlTableModel::OnManualSubmit);
    model2.setHeaderData(1, Qt::Horizontal, "Наименование");
    model2.setHeaderData(3, Qt::Horizontal, "Срок гарантии (мес.)");
    model2.setHeaderData(4, Qt::Horizontal, "Децимальный номер");

    ui->tableView->setModel(&model2);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setColumnWidth(1, 400);

    ui->tableView->selectRow(0);

}

ProdVNFTWindow::~ProdVNFTWindow()
{
    delete ui;
}


//------------------------------------------------------------------------------------------------
// Добавление строки
//------------------------------------------------------------------------------------------------
void ProdVNFTWindow::on_tbAdd_clicked()
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
void ProdVNFTWindow::on_tbDelete_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    model2.removeRow(index.row());
}

void ProdVNFTWindow::on_pbSave_clicked()
{
    bool res = model2.submitAll();
    if(!res)
        QMessageBox::critical(this, "Ошибка", "Произошла ошибка при сохранении базы данных.");
}


void ProdVNFTWindow::on_pbCancel_clicked()
{
    model2.revertAll();
}

