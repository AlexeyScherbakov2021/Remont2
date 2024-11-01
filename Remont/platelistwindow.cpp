#include "platelistwindow.h"
#include "ui_platelistwindow.h"

#include <qmessagebox.h>

PlateListWindow::PlateListWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateListWindow)
{
    ui->setupUi(this);
    RepoMSSQL repo;
    repo.LoadTypePlate(listType);
    for(auto &it : listType)
        listPlateType.insert(it.id, it.VNFT);

    listPlate.FindItems("");
    LinkTypePlate();

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
    LinkTypePlate();
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


//---------------------------------------------------------------------------------------
// Привязка строки ВНФТ
//---------------------------------------------------------------------------------------
void PlateListWindow::LinkTypePlate()
{
    for(auto &it : listPlate.listItems)
    {
        if(it.idType > 0)
        {
            QString VNFT = listPlateType.value(it.idType);
            it.VNFT = VNFT;
        }
    }

}


//---------------------------------------------------------------------------------------
// Удаление выделенной строки
//---------------------------------------------------------------------------------------
void PlateListWindow::on_pbDelete_clicked()
{
    int row = ui->twPlates->currentRow();
    if(row < 0)
        return;

    if(listPlate.listItems[row].idParent > 0)
    {
        QMessageBox::critical(this, "Ошибка", "Плата используется в модуле. Удалить нельзя.");
        return;
    }


    if(QMessageBox::warning(this, "Предупреждение",
                             QString("Удалить \"%1\"").arg(listPlate.listItems[row].number), QMessageBox::No | QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        RepoMSSQL repo;
        if(repo.DeletePlate(listPlate.listItems[row].id))
        {
            listPlate.listItems.removeAt(row);
            ui->twPlates->removeRow(row);
        }
    }

}

