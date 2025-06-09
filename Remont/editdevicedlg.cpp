#include "editdevicedlg.h"
#include "repomssql.h"
#include "selectdevicewindow.h"
#include "statusdlg.h"
#include "ui_editdevicedlg.h"

#include <QMessageBox>

EditDeviceDlg::EditDeviceDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDeviceDlg)
{
    ui->setupUi(this);
    ui->deCreate->setNullDate(QDate(1900,1,1));
    ui->deDateGarant->setNullDate(QDate(1900,1,1));
    ui->twHistory->horizontalHeader()->resizeSection(0, 110);
    ui->twHistory->horizontalHeader()->resizeSection(1, 166);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
EditDeviceDlg::~EditDeviceDlg()
{
    delete ui;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void EditDeviceDlg::on_tbSearch_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    win->AddSelectedType(IndexType::Plate);
    QVector<StatusItem> stat;
    Items *dev = win->SelectDevice(true, stat, ui->leSearch->text(), true, LoadPartType::ANY_PARENT);
    if(dev != nullptr)
    {
        // qDebug() << dev->number;
        device = *dev;
        device.LoadStatus(device);

        ui->leNumber->setText(device.number);
        ui->leNumber2->setText(device.number2);
        ui->deCreate->setDateTime(device.dateCreate);
        ui->deDateGarant->setDateTime(device.dateGarant);
        ui->leName->setText(device.name);
        ui->leNumDoc->setText(device.numberDoc);
        ui->leDop->setText(device.descript);

        ShowStatus();
    }
}


//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void EditDeviceDlg::on_pbSaveItem_clicked()
{
    if(device.id == 0)
        return;

    if(ui->deCreate->isNull())
        device.dateCreate = QDateTime::fromString("00.00.0000","dd.MM.yyyy");
    else
        device.dateCreate = ui->deCreate->dateTime();

    if(ui->deDateGarant->isNull())
        device.dateGarant = QDateTime::fromString("00.00.0000","dd.MM.yyyy");
    else
        device.dateGarant = ui->deDateGarant->dateTime();

    device.number = ui->leNumber->text();
    device.number2 = ui->leNumber2->text();
    device.name = ui->leName->text();
    device.numberDoc = ui->leNumDoc->text();
    device.descript = ui->leDop->text();

    repo.UpdateItem(device);
}


//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void EditDeviceDlg::on_pbAddStatus_clicked()
{
    if(device.id == 0)
        return;

    Status stat;
    stat.idItem = device.id;
    StatusDlg *win = new StatusDlg(stat, this);
    if(win->exec() == QDialog::Accepted)
    {
        int row = ui->twHistory->rowCount();
        ui->twHistory->insertRow(row);

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(stat.dateStatus.toString("dd.MM.yyyy hh:mm:ss"));
        ui->twHistory->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setText(stat.nameStatus);
        ui->twHistory->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setText(stat.numberDoc);
        ui->twHistory->setItem(row, 2, item);

        item = new QTableWidgetItem();
        item->setText(stat.Comment);
        ui->twHistory->setItem(row, 3, item);

        device.AddStatus(device, stat);
    }
}


//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void EditDeviceDlg::on_pbDeleteStatus_clicked()
{
    if(device.id == 0 || device.listStatus.size() == 0)
        return;

    Status stat = device.listStatus.last();

    if( QMessageBox::warning(this, "Предупреждение",
                             "Будет удален последний статус '" + stat.nameStatus + "'", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        device.DeleteLastStatus(device, stat.idStatus);
        ui->twHistory->removeRow(ui->twHistory->rowCount() - 1);
    }
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void EditDeviceDlg::ShowStatus()
{
    ui->twHistory->setRowCount(device.listStatus.size());

    int row = 0;
    for(auto &it : device.listStatus)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(it.dateStatus.toString("dd.MM.yyyy hh:mm:ss"));
        ui->twHistory->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setText(it.nameStatus);
        ui->twHistory->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setText(it.numberDoc);
        ui->twHistory->setItem(row, 2, item);

        item = new QTableWidgetItem();
        item->setText(it.Comment);
        ui->twHistory->setItem(row, 3, item);

        ++row;
    }
}

