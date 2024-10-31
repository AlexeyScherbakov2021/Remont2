#include "platelistwindow.h"
#include "ui_platelistwindow.h"

PlateListWindow::PlateListWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateListWindow)
{
    ui->setupUi(this);
    listPlate.FindItems("");
    UpdateForm();

}

PlateListWindow::~PlateListWindow()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
// Кнопака поиска
//---------------------------------------------------------------------------------------
void PlateListWindow::on_tbSearch_clicked()
{
    listPlate.FindItems(ui->leSearch->text());
    UpdateForm();
}

//---------------------------------------------------------------------------------------
// Обновление формы
//---------------------------------------------------------------------------------------
void PlateListWindow::UpdateForm()
{
    ui->twPlates->setRowCount(listPlate.listItems.size());

    int row;
    for(auto it : listPlate.listItems)
    {
        QTableWidgetItem *item = new QTableWidgetItem(it.number);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->twPlates->setItem(row, 0, item);

        item = new QTableWidgetItem(it.number2);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->twPlates->setItem(row, 1, item);

        item = new QTableWidgetItem(it.VNFT);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->twPlates->setItem(row, 2, item);

        item = new QTableWidgetItem(it.dateRegister.toString("dd.MM.yyyy"));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->twPlates->setItem(row, 3, item);

        item = new QTableWidgetItem(it.numberDoc);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->twPlates->setItem(row, 4, item);

        item = new QTableWidgetItem();
        // item->setText(QString::number(it.idParent));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignCenter);
        if(it.idParent > 0)
            item->setIcon(QIcon("://image/Apply24x24.png"));
        ui->twPlates->setItem(row, 5, item);

        ++row;
    }
    ui->twPlates->resizeColumnsToContents();
    ui->twPlates->resizeRowsToContents();
}

