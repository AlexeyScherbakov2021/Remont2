#include "scan.h"
#include "selectdevicewindow.h"
#include "startworkwindow.h"
#include "ui_startworkwindow.h"

#include <QMessageBox>

#include <models/claim.h>
#include <models/remont.h>

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
    ui->pbProdToWork->setEnabled(false);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void StartWorkWindow::SetStatusAllDevice(Items *item, QDateTime& dateOn)
{
    if(item->listStatus.last().idStatus == StatusItem::WORK)
        return;

    item->dateOn = dateOn;
    item->dateGarant = dateOn.addMonths(item->garantMonth);
    repo.UpdateItem(*item);
    item->AddStatus(*item, StatusItem::WORK, dateOn, "документ № " + ui->leDoc->text());
    repo.LoadChildItems(item->id, item->childItems);
    for(auto &it : item->childItems)
        SetStatusAllDevice(&it, dateOn);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void StartWorkWindow::SelectDevice()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    win->ExcludeDevice(listAddId);

    // Items *dev = win->SelectDevice(true, {StatusItem::CORRECT_OSO}, ui->leSearch->text(), false, true);
    Items *dev = win->SelectDevice(true, {StatusItem::SHIPPED}, ui->leSearch->text(), true);
    if(dev != nullptr && dev->id > 0)
    {
        AddDevice(dev);
        listAddId.insert(dev->id);
    }
}


//----------------------------------------------------------------------------
// Поиск оборудования
//----------------------------------------------------------------------------
void StartWorkWindow::on_tbSearch_clicked()
{
    SelectDevice();
    // SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    // win->AddSelectedType(IndexType::Modul);

    // // Items *dev = win->SelectDevice(true, {StatusItem::CORRECT_OSO}, ui->leSearch->text(), false, true);
    // Items *dev = win->SelectDevice(true, {StatusItem::SHIPPED}, ui->leSearch->text(), true);
    // if(dev != nullptr && dev->id > 0)
    // {
    //     AddDevice(dev);
    // }
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

    int id = ui->lwProduct->item(row, 0)->data(Qt::UserRole).toInt();
    listAddId.remove(id);

    ui->lwProduct->removeRow(row);
    listDev.removeAt(row);

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











EnterWorkWindow::EnterWorkWindow(QWidget *parent) : StartWorkWindow(parent)
{
    setWindowTitle("Ввести в работу после ремонта");
}


void EnterWorkWindow::SelectDevice()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    win->ExcludeDevice(listAddId);

    Items *dev = win->SelectDevice(true, {StatusItem::CORRECT_OSO}, ui->leSearch->text(), false, LoadPartType::HAS_PARENT);
    // Items *dev = win->SelectDevice(true, {StatusItem::SHIPPED}, ui->leSearch->text(), true);
    if(dev != nullptr && dev->id > 0)
    {
        AddDevice(dev);
        listAddId.insert(dev->id);
    }
}



void EnterWorkWindow::SetStatusAllDevice(Items *item, QDateTime &dateOn)
{
    if(item->listStatus.last().idStatus == StatusItem::WORK)
        return;

    // item->dateOn = dateOn;
    // item->dateGarant = dateOn.addMonths(item->garantMonth);
    // repo.UpdateItem(*item);
    item->AddStatus(*item, StatusItem::WORK, dateOn, ui->leDoc->text());

    Claim claim = repo.GetClaimForItem(item->id);
    claim.CheckAndClose();

    // repo.LoadChildClaim(claim);
    // bool resClaim = true;
    // foreach (const Items it, claim.childItems)
    // {
    //     resClaim &= it.listStatus.last().idStatus == StatusItem::WORK;
    // }
    // if(resClaim)
    // {
    //     claim.isClosed = true;
    //     repo.UpdateItem(claim);
    // }


    Items parent = repo.GetItem(item->idParent);
    while(parent.id > 0 && claim.id > 0)
    {
        bool res = parent.TestChildStatus(StatusItem::CORRECT_OSO);
        if(res)
        {
            parent.AddStatus(parent, StatusItem::WORK);
            // Claim claim = repo.GetClaimForItem(parent.id);
            // Q_ASSERT(claim.id != 0);
            // if(claim.id > 0)
            // {
                Remont remParent = repo.GetRemontForItem(parent.id, claim.id);
                Q_ASSERT(remParent.id != 0);
                remParent.action = "";
                remParent.defect = "Неисправные комплектующие";
                remParent.endDate = ui->deDate->dateTime();
                remParent.idReason = 2;
                remParent.remark = "Комплектующие исправны";
                repo.UpdateRemont(remParent);
            // }
        }
        parent = repo.GetItem(parent.idParent);
    }


    // repo.LoadChildItems(item->id, item->childItems);
    // for(auto &it : item->childItems)
    //     SetStatusAllDevice(&it, dateOn);
}

