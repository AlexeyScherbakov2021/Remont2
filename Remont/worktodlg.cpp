#include "worktodlg.h"
#include "ui_worktodlg.h"
#include "selectdevicewindow.h"
#include "changemoduldlg.h"
#include "platefwwindow.h"

#include <QMessageBox>

#include <models/remont.h>

workTODlg::workTODlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::workTO)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

workTODlg::~workTODlg()
{
    delete ui;
}


//--------------------------------------------------------------------------------------------------
// поиск устройства
//--------------------------------------------------------------------------------------------------
void workTODlg::on_tbSearch_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    win->AddSelectedType(IndexType::Plate);

    // win->ExcludeDevice(listAddId);

    Items *dev = win->SelectDevice(true, {StatusItem::SHIPPED, StatusItem::WORK}, ui->leSearch->text(), true, LoadPartType::ANY_PARENT);
    if(dev != nullptr && dev->id > 0)
    {
        ui->treeDevice->Clear();

        mainDev = *dev;
        while(mainDev.idParent > 0)
            mainDev = repo.GetItem(dev->idParent);

        ui->lbGarant->setText(mainDev.dateGarant.toString("dd.MM.yyyy"));

        if(mainDev.idSet != 0)
        {
            setter = repo.GetSetter(mainDev.idSet);
            ui->treeDevice->AddItem(&setter);
        }
        else
            ui->treeDevice->AddItem(&mainDev);
        ui->treeDevice->SetSelectItem(dev->id, dev->type.indexType);
        ui->treeDevice->setExpand();

        qDebug() << &mainDev;

    }
}


//--------------------------------------------------------------------------------------------------
// Замена модуля
//--------------------------------------------------------------------------------------------------
void workTODlg::on_pbExchange_clicked()
{
    auto sel = ui->treeDevice->GetSelectedItem();

    if(sel.first == 0)
        return;

    Items dev = repo.GetItem(sel.first);
    if(sel.second == IndexType::Modul || sel.second == IndexType::Plate)
    {
        ChangeModulDlg *win = new ChangeModulDlg(&dev, this, true);
        if(win->exec() == QDialog::Accepted)
        {
            ui->treeDevice->Clear();
            // Items parent = dev;
            // while(parent.idParent > 0)
            //     parent = repo.GetItem(dev.idParent);
            // ui->treeDevice->AddItem(&parent);
            // ui->treeDevice->SetSelectItem(dev.id, dev.type.indexType);

            ui->treeDevice->AddItem(&mainDev);

            Remont remont = repo.GetCurrentRemontForItem(dev.id);
            remont.regDate = ui->dateEdit->dateTime();
            repo.UpdateRemont(remont);
        }
    }
}


//--------------------------------------------------------------------------------------------------
// Замена прошивки
//--------------------------------------------------------------------------------------------------
void workTODlg::on_pbFirmware_clicked()
{
    auto sel = ui->treeDevice->GetSelectedItem();
    if(sel.first == 0)
        return;

    Items dev = repo.GetItem(sel.first);

    if(sel.second == IndexType::Plate || sel.second == IndexType::Modul)
    {
        PlateFWWindow *win = new PlateFWWindow(this, &dev);
        win->exec();
    }
}



//--------------------------------------------------------------------------------------------------
// Продление гарантии
//--------------------------------------------------------------------------------------------------
void workTODlg::on_pbGarantLong_clicked()
{
    int month = ui->cbGarant->currentText().toInt();
    if(month <= 0)
        return;

    QDateTime newGarantDate = ui->dateEdit->dateTime().addMonths(month);
    QString stringDate = newGarantDate.toString("dd.MM.yyyy");


    if(QMessageBox::question(this, "Предупреждение", "Новый срок гарантии будет " + stringDate, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        ui->lbGarant->setText(stringDate);
        // mainDev.dateGarant = newGarantDate;

        if(mainDev.idSet > 0)
        {
            SetterOut setter = repo.GetSetter(mainDev.idSet);
            repo.LoadChildSetter(setter);

            for(auto &it : setter.childItems)
            {
                SetNewGarantDate(&it, newGarantDate);
            }
        }
        else
            SetNewGarantDate(&mainDev, newGarantDate);


    }
}

//--------------------------------------------------------------------------------------------------
// установка нового срока гарантии для устройства и его содержимого
//--------------------------------------------------------------------------------------------------
void workTODlg::SetNewGarantDate(Items *dev, QDateTime &date)
{
    dev->dateGarant = date;
    repo.UpdateItem(*dev);

    repo.LoadChildItems(dev->id, dev->childItems);
    for(auto &it : dev->childItems)
        SetNewGarantDate(&it, date);

}

