#include "platelistwindow.h"
#include "scan.h"
#include "ui_platelistwindow.h"

#include <QElapsedTimer>
#include <QInputDialog>
#include <QMenu>
#include <qmessagebox.h>

PlateListWindow::PlateListWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlateListWindow)
{

    ui->setupUi(this);
    ui->pbSelect->setVisible(isSelectPlate);

    model = new DeviceModel(ItemType::Plate, this);
    startLoad();
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(2, 200);
    // ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 20);
    ui->tableView->setColumnWidth(6, 150);

    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

PlateListWindow::~PlateListWindow()
{
    disconnect(conn);
    delete ui;
}

Items PlateListWindow::SelectPlate(QString number)
{
    Items plate;

    ui->leSearch->setText(number);

    if(listExcludePlate == nullptr)
    {
        if(isNotLinked)
            on_rbNotLink_clicked();
        else
            on_rbAll_clicked();
    }
    // else
    //     listPlate.FindItemsExclude(number, *listExcludePlate);

    LinkTypePlate();

    // if(!number.isEmpty() && listPlate.items.size() == 1)
    // {
    //     return listPlate.items[0];
    // }

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

void PlateListWindow::setSelectForm()
{
    isSelectPlate = true;
    UpdateForm();
}

void PlateListWindow::setNotLinked()
{
    isNotLinked = true;
}

void PlateListWindow::RemoveListPlate(QList<Items> &listExclude)
{
    listExcludePlate = &listExclude;
}

//---------------------------------------------------------------------------------------
// Кнопка поиска
//---------------------------------------------------------------------------------------
void PlateListWindow::on_tbSearch_clicked()
{
    startLoad();
}

//---------------------------------------------------------------------------------------
// Выбор непривязанных плат
//---------------------------------------------------------------------------------------
void PlateListWindow::on_rbNotLink_clicked()
{
    startLoad();
}


//---------------------------------------------------------------------------------------
// Выбор всех плат
//---------------------------------------------------------------------------------------
void PlateListWindow::on_rbAll_clicked()
{
    startLoad();
}

//---------------------------------------------------------------------------------------
// Начало нового запроса
//---------------------------------------------------------------------------------------
void PlateListWindow::startLoad()
{
    model->prepareLoad(ui->leSearch->text(), 0, false, ui->rbAll->isChecked() );
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
}


//---------------------------------------------------------------------------------------
// Привязка строки ВНФТ
//---------------------------------------------------------------------------------------
void PlateListWindow::LinkTypePlate()
{
    // for(auto &it : listPlate.items)
    // {
    //     if(it.idType > 0)
    //     {
    //         QString VNFT = listPlateType.value(it.idType);
    //         it.VNFT = VNFT;
    //     }
    // }
}


//---------------------------------------------------------------------------------------
// Удаление выделенной строки
//---------------------------------------------------------------------------------------
void PlateListWindow::on_pbDelete_clicked()
{

    int row = ui->tableView->currentIndex().row();

    if(row < 0)
        return;

    Items* plate = model->GetItem(row);

    if(plate->idParent > 0)
    {
        QMessageBox::critical(this, "Ошибка", "Плата используется в модуле. Удалить нельзя.");
        return;
    }

    if(QMessageBox::warning(this, "Предупреждение",
                             QString("Удалить \"%1\"").arg(plate->number), QMessageBox::No | QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {

        model->DeleteItem(row);
    }

}


void PlateListWindow::on_pbSelect_clicked()
{
    auto selected = ui->tableView->selectionModel()->selectedRows();

    if(selected.size() == 0)
        return;

    for(auto item : selected)
    {
        // qDebug() << model->GetItem(item.row())->number;
        Items *plate = model->GetItem(item.row());
        selectedPlates.push_back(*plate);
    }

    int row = ui->tableView->selectionModel()->currentIndex().row();
    selectPlate = *model->GetItem(row);
    // qDebug() << selectPlate.number;

    accept();

    // QList<QTableWidgetSelectionRange> items = ui->twPlates->selectedRanges();

    // if(items.size() > 0)
    // {

    //     for(auto &it : items)
    //     {
    //         for(int row = it.topRow(); row <= it.bottomRow(); ++row)
    //         {
    //             int id = ui->twPlates->item(row, 0)->data(Qt::UserRole).toInt();
    //             Items plate = listPlate.GetItem(id);
    //             selectedPlates.push_back(plate);
    //             // qDebug() << plate.number;
    //         }
    //     }
    //     int id = ui->twPlates->item(ui->twPlates->currentRow(), 0)->data(Qt::UserRole).toInt();
    //     selectPlate = listPlate.GetItem(id);

    //     accept();
    // }

}


// void PlateListWindow::on_twPlates_itemDoubleClicked(QTableWidgetItem *item)
// {
//     if(!isSelectPlate)
//         return;
//     // int id = ui->twPlates->item(item->row(), 0)->data(Qt::UserRole).toInt();
//     // selectPlate = listPlate.GetItem(id);
//     // selectedPlates.push_back(listPlate.GetItem(id));
//     accept();

// }

void PlateListWindow::slotReadScan(QString s)
{
    ui->leSearch->setText(s);
    on_tbSearch_clicked();
}


// void PlateListWindow::on_actionBroken_triggered()
// {
//     int row = ui->twPlates->currentRow();
//     if(row < 0)
//         return;

//     int id = ui->twPlates->item(row, 0)->data(Qt::UserRole).toInt();
//     Items plate = listPlate.GetItem(id);
//     if(plate.listStatus.size() > 0)
//         return;

//     QString comment = QInputDialog::getText(this, "Ввод текста", "Введите комментарий: ");
//     plate.AddStatus(plate, Status::FAULTY, QDateTime::currentDateTime(), comment);

//     QTableWidgetItem *item = new QTableWidgetItem(plate.listStatus.last().nameStatus);
//     qDebug() << plate.listStatus.last().nameStatus;
//     item->setToolTip(comment);
//     ui->twPlates->setItem(row, 6, item);
// }


// void PlateListWindow::on_twPlates_customContextMenuRequested(const QPoint &pos)
// {
//     QMenu menu(this);
//     menu.addAction("Удалить", this, SLOT(on_pbDelete_clicked()));
    // menu.addAction("Забраковать", this, SLOT(on_actionBroken_triggered()));
    // menu.exec(ui->twPlates->viewport()->mapToGlobal(pos));
// }

