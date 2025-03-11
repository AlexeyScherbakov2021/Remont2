#include "scan.h"
#include "selectdevicewindow.h"
#include "startworkwindow.h"
#include "ui_startworkwindow.h"

#include <QMessageBox>

StartWorkWindow::StartWorkWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartWorkWindow)
{
    ui->setupUi(this);

    ui->deDate->setNullDate(QDate(1900,1,1));
    ui->deDate->setDateTime(QDateTime::currentDateTime());
    ui->lwProduct->setColumnWidth(0, 400);
    ui->lwProduct->setColumnWidth(1, 150);
    connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

StartWorkWindow::~StartWorkWindow()
{
    delete ui;
}


//----------------------------------------------------------------------------
// Кнопка ввести в работу
//----------------------------------------------------------------------------
void StartWorkWindow::on_pbProdToWork_clicked()
{
    // auto item = ui->lwProduct->currentItem();
    // if(item == nullptr)
    //     return;
    if(listDev.size() == 0)
        return;

    if(ui->leDoc->text().isEmpty())
    {
        QMessageBox::information(this, "Сообщение", "Укажите документ.");
        return;
    }

    if(ui->deDate->isNull())
    {
        QMessageBox::information(this, "Сообщение", "Не указана дата.");
        return;
    }

    QDateTime dateOn = ui->deDate->dateTime();

    for(auto &it : listDev)
    {
        SetStatusAllDevice(&it, dateOn);
        // it.dateOn = dateOn;
        // it.dateGarant = dateOn.addMonths(it.garantMonth);
        // repo.UpdateItem(it);
        // it.AddStatus(it, StatusItem::WORK, dateOn, ui->leDoc->text());
    }

    QString statusName = listDev.first().getNameLastStatus();
    for(int row = 0; row < ui->lwProduct->rowCount(); ++row)
    {
        QTableWidgetItem *item = ui->lwProduct->item(row, 1);
        item->setText(statusName);

        item = new QTableWidgetItem();
        item->setText(dateOn.toString("dd.MM.yyyy"));
        ui->lwProduct->setItem(row, 2, item);
    }

    ui->tbDelete->setEnabled(false);
    ui->tbSearch->setEnabled(false);
}

void StartWorkWindow::SetStatusAllDevice(Items *item, QDateTime& dateOn)
{
    item->dateOn = dateOn;
    item->dateGarant = dateOn.addMonths(item->garantMonth);
    repo.UpdateItem(*item);
    item->AddStatus(*item, StatusItem::WORK, dateOn, ui->leDoc->text());
    repo.LoadChildItems(item->id, item->childItems);
    for(auto &it : item->childItems)
        SetStatusAllDevice(&it, dateOn);
}


//----------------------------------------------------------------------------
// Поиск оборудования
//----------------------------------------------------------------------------
void StartWorkWindow::on_tbSearch_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);

    Items *dev = win->SelectDevice(true, {StatusItem::SHIPPED}, ui->leSearch->text(), true);
    if(dev != nullptr && dev->id > 0)
    {
        AddDevice(dev);
        // repo.LoadStatus(*dev);
        // int row = ui->lwProduct->rowCount();
        // ui->lwProduct->insertRow(row);
        // QString nameType, nameIcon;
        // dev->GetInfo(nameType, nameIcon);

        // QTableWidgetItem *item = new QTableWidgetItem();
        // item->setText(dev->GetDefaultName());
        // item->setIcon(QIcon(nameIcon));
        // item->setData(Qt::UserRole, dev->id);
        // ui->lwProduct->setItem(row, 0, item);

        // item = new QTableWidgetItem();
        // item->setText(dev->getNameLastStatus());
        // ui->lwProduct->setItem(row, 1, item);

        // listDev.push_back(*dev);
    }
}



void StartWorkWindow::AddDevice(Items* dev)
{
    repo.LoadStatus(*dev);
    int row = ui->lwProduct->rowCount();
    ui->lwProduct->insertRow(row);
    QString nameType, nameIcon;
    dev->GetInfo(nameType, nameIcon);

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(dev->GetDefaultName());
    item->setIcon(QIcon(nameIcon));
    item->setData(Qt::UserRole, dev->id);
    ui->lwProduct->setItem(row, 0, item);

    item = new QTableWidgetItem();
    item->setText(dev->getNameLastStatus());
    ui->lwProduct->setItem(row, 1, item);

    listDev.push_back(*dev);
}


//----------------------------------------------------------------------------
// Кнопка Удалить оборудование
//----------------------------------------------------------------------------
void StartWorkWindow::on_tbDelete_clicked()
{
    int row = ui->lwProduct->currentRow();
    if(row < 0)
        return;

    ui->lwProduct->removeRow(row);
    listDev.removeAt(row);

    // auto ranges = ui->lwProduct->selectedRanges();

    // for(auto &it : ranges)
    // {
    //     for(int row = it.topRow(); row <= it.bottomRow(); ++row)
    //     {
    //         int id = ui->lwProduct->item(row, 0)->data(Qt::UserRole).toInt();
    //         listDev.removeIf([id] (Items dev) { return dev.id == id; } );

    //     }
    //     ranges.removeAll;
    // }
}

void StartWorkWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        RepoMSSQL repo;
        Items item = repo.GetItem2(s, { StatusItem::SHIPPED}, true);

        if(item.id != 0)
        {
            ui->leSearch->setText(s);
            AddDevice(&item);
        }
    }
}

