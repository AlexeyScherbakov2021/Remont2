#include "modvnftwindow.h"
#include "ui_modvnftwindow.h"

#include <models/modultype.h>

#include <qmessagebox.h>

ModVNFTWindow::ModVNFTWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModVNFTWindow)
{
    ui->setupUi(this);

    model2.setTable("ModuleType");
    model2.select();
    model2.setEditStrategy(QSqlTableModel::OnManualSubmit);
    model2.setHeaderData(2, Qt::Horizontal, "Наименование");
    model2.setHeaderData(4, Qt::Horizontal, "Срок гарантии (мес.)");
    model2.setHeaderData(5, Qt::Horizontal, "Децимальный номер");

    ui->tableView->setModel(&model2);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(3);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->selectRow(0);

}

ModVNFTWindow::~ModVNFTWindow()
{
    delete ui;
}


//------------------------------------------------------------------------------------------------
// Добавление строки
//------------------------------------------------------------------------------------------------
void ModVNFTWindow::on_tbAdd_clicked()
{
    int row = model2.rowCount();
    model2.insertRow(row);
    ui->tableView->selectRow(row);
    ui->tableView->edit(model2.index(row, 2));
    ui->tableView->resizeRowToContents(row);
}


//------------------------------------------------------------------------------------------------
// Удаление строки
//------------------------------------------------------------------------------------------------
void ModVNFTWindow::on_tbDelete_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    model2.removeRow(index.row());
}

void ModVNFTWindow::on_pbSave_clicked()
{
    bool res = model2.submitAll();
    if(!res)
        QMessageBox::critical(this, "Ошибка", "Произошла ошибка при сохранении базы данных.");
}


void ModVNFTWindow::on_pbCancel_clicked()
{
    model2.revertAll();
}

