#include "platelistwindow.h"
#include "scan.h"
#include "ui_platelistwindow.h"

#include <QInputDialog>
#include <QMenu>
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

    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

PlateListWindow::~PlateListWindow()
{
    disconnect(conn);
    delete ui;
}

Plate PlateListWindow::SelectPlate(QString number)
{
    Plate plate;

    ui->leSearch->setText(number);

    if(listExcludePlate == nullptr)
    {
        if(isNotLinked)
            on_rbNotLink_clicked();
        else
            on_rbAll_clicked();
    }
    else
        listPlate.FindItemsExclude(number, *listExcludePlate);

    LinkTypePlate();

    if(!number.isEmpty() && listPlate.listItems.size() == 1)
    {
        return listPlate.listItems[0];
    }

    if(isSelectPlate && !number.isEmpty())
        return plate;


    UpdateForm();

    if(exec() == QDialog::Accepted)
    {
        return selectPlate;
        // int row = ui->twPlates->currentRow();
        // if(row >= 0)
        // {
        //     int id = ui->twPlates->item(row, 0)->data(Qt::UserRole).toInt();
        //     plate = listPlate.GetItem(id);
        // }
    }

    return plate;
}

void PlateListWindow::setSelect()
{
    isSelectPlate = true;
}

void PlateListWindow::setNotLinked()
{
    isNotLinked = true;
}

void PlateListWindow::RemoveListPlate(QList<Plate> &listExclude)
{
    listExcludePlate = &listExclude;
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
    ui->pbSelect->setVisible(isSelectPlate);
    ui->pbDelete->setVisible(!isSelectPlate);
    ui->rbAll->setVisible(!isNotLinked);
    ui->rbNotLink->setVisible(!isNotLinked);

    ui->twPlates->setRowCount(listPlate.listItems.size());
    // ui->twPlates->setRowCount(10);

    int row = 0;

    for(auto &it : listPlate.listItems)
    {
        QTableWidgetItem *item = new QTableWidgetItem(it.number);
        item->setData(Qt::UserRole, it.id);
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
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        if(it.idParent > 0)
            item->setIcon(QIcon("://image/Apply24x24.png"));
        ui->twPlates->setItem(row, 5, item);

        if(it.listStatus.size() > 0)
        {
            item = new QTableWidgetItem(it.listStatus.last().nameStatus);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setToolTip(it.listStatus.last().Comment);
            ui->twPlates->setItem(row, 6, item);
        }

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


//---------------------------------------------------------------------------------------
// Выбор непривязанных плат
//---------------------------------------------------------------------------------------
void PlateListWindow::on_rbNotLink_clicked()
{
    listPlate.FindItems("", 0, true);
    LinkTypePlate();
    UpdateForm();
}


//---------------------------------------------------------------------------------------
// Выбор всех плат
//---------------------------------------------------------------------------------------
void PlateListWindow::on_rbAll_clicked()
{
    listPlate.FindItems("");
    LinkTypePlate();
    UpdateForm();
}


void PlateListWindow::on_pbSelect_clicked()
{
    QList<QTableWidgetSelectionRange> items = ui->twPlates->selectedRanges();

    if(items.size() > 0)
    {

        for(auto &it : items)
        {
            for(int row = it.topRow(); row <= it.bottomRow(); ++row)
            {
                int id = ui->twPlates->item(row, 0)->data(Qt::UserRole).toInt();
                Plate plate = listPlate.GetItem(id);
                selectedPlates.push_back(plate);
                // qDebug() << plate.number;
            }
        }
        int id = ui->twPlates->item(ui->twPlates->currentRow(), 0)->data(Qt::UserRole).toInt();
        selectPlate = listPlate.GetItem(id);

        accept();
        // return;
    }
    // else
    // {
    //     int row = ui->twPlates->currentRow();
    //     if(row >= 0)
    //     {
    //         on_twPlates_itemDoubleClicked(ui->twPlates->item(row, 0));
    //     }
    // }

}


void PlateListWindow::on_twPlates_itemDoubleClicked(QTableWidgetItem *item)
{
    if(!isSelectPlate)
        return;
    int id = ui->twPlates->item(item->row(), 0)->data(Qt::UserRole).toInt();
    selectPlate = listPlate.GetItem(id);
    selectedPlates.push_back(listPlate.GetItem(id));
    accept();

}

void PlateListWindow::slotReadScan(QString s)
{
    ui->leSearch->setText(s);
    on_tbSearch_clicked();
}


void PlateListWindow::on_actionBroken_triggered()
{
    int row = ui->twPlates->currentRow();
    if(row < 0)
        return;

    int id = ui->twPlates->item(row, 0)->data(Qt::UserRole).toInt();
    Plate plate = listPlate.GetItem(id);
    if(plate.listStatus.size() > 0)
        return;

    QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");
    plate.AddStatus(plate, Status::FAULTY, QDateTime::currentDateTime(), comment);

    QTableWidgetItem *item = new QTableWidgetItem(plate.listStatus.last().nameStatus);
    qDebug() << plate.listStatus.last().nameStatus;
    item->setToolTip(comment);
    ui->twPlates->setItem(row, 6, item);
}


void PlateListWindow::on_twPlates_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction("Удалить", this, SLOT(on_pbDelete_clicked()));
    menu.addAction("Забраковать", this, SLOT(on_actionBroken_triggered()));
    menu.exec(ui->twPlates->viewport()->mapToGlobal(pos));
}
