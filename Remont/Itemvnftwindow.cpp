#include "Itemvnftwindow.h"
#include "ui_Itemvnftwindow.h"
// #include <models/platetype.h>
#include <qmessagebox.h>
#include <QElapsedTimer>

ItemVNFTWindow::ItemVNFTWindow(IndexType t, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ItemVNFTWindow), workType(t)
{
    ui->setupUi(this);

    setWindowTitle("Справочник обозначений для " + nameWindow[workType]);

    model2.setTable("ItemType");
    model2.setFilter(QString("indexType=%1").arg(workType));

    ui->tableView->setModel(&model2);
    model2.select();

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    model2.setEditStrategy(QSqlTableModel::OnManualSubmit);
    model2.setHeaderData(2, Qt::Horizontal, "Наименование");
    model2.setHeaderData(3, Qt::Horizontal, "Гарантия(мес.)");
    model2.setHeaderData(4, Qt::Horizontal, "Децимальный номер");

    ui->tableView->setColumnWidth(2, 450);
    ui->tableView->setColumnWidth(3, 70);
    ui->tableView->setColumnWidth(4, 200);

    // ui->tableView->resizeColumnsToContents();
    // ui->tableView->resizeRowsToContents();
    ui->tableView->selectRow(0);

}

ItemVNFTWindow::~ItemVNFTWindow()
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
void ItemVNFTWindow::on_tbAdd_clicked()
{
    int row = model2.rowCount();
    model2.insertRow(row);
    ui->tableView->selectRow(row);
    ui->tableView->edit(model2.index(row, 2));
    ui->tableView->resizeRowToContents(row);

    // QModelIndex index = ui->tableView->currentIndex();
    QModelIndex index = model2.index(row, 1);
    model2.setData(index, workType);
}


//------------------------------------------------------------------------------------------------
// Удаление строки
//------------------------------------------------------------------------------------------------
void ItemVNFTWindow::on_tbDelete_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    model2.removeRow(index.row());
}

void ItemVNFTWindow::on_pbSave_clicked()
{

    bool res = model2.submitAll();
    if(!res)
        QMessageBox::critical(this, "Ошибка", "Произошла ошибка при сохранении базы данных.");
}


void ItemVNFTWindow::on_pbCancel_clicked()
{
    model2.revertAll();
}

