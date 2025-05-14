#include "cardprodwindow.h"
#include "changetypedlg.h"
#include "logindlg.h"
#include "shipwindow.h"
#include "ui_cardprodwindow.h"
#include <models/claim.h>
#include <models/listproduct.h>
#include <models/remont.h>
#include <models/setterout.h>
#include <models/shipment.h>
#include <QClipboard>
#include <QSqlQueryModel>


//-------------------------------------------------------------------------------------------------------
// Конструктор
//-------------------------------------------------------------------------------------------------------
CardProdWindow::CardProdWindow(Items *_device, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CardProdWindow), device(_device)
{
    ui->setupUi(this);

    SetterOut setter;
    Items root;

    setWindowTitle("Карточка \"" + device->GetDefaultName() + "\"");

    ui->tbChangeType->setVisible(LoginDlg::CurrUser.listRoles.contains(RolesType::EditItems));

    QString nameType, iconType;
    device->GetInfo(nameType, iconType);
    device->LoadStatus(*device);

    LoadRemontToForm(device->id);

    loadInclude(device);
    LoadHistoryToForm(device->listStatus);

    root = *device;
    while(root.idParent > 0)
    {
        root = repo.GetItem(root.idParent);
    }

    if(root.idShip > 0)
        ship = repo.GetShipment(root.idShip);
    if(root.idSet > 0)
        setter = repo.GetSetter(root.idSet);
    if(setter.idShip > 0)
        ship = repo.GetShipment(setter.idShip);

    ui->pbToShip->setEnabled(ship.id > 0);

    ui->lbGarant->setText(device->dateGarant.toString("dd.MM.yyyy"));
    ui->lbDateCreate->setText(device->dateCreate.toString("dd.MM.yyyy"));
    ui->lbNumber->setText(device->number);
    ui->lbDateOn->setText(device->dateOn.toString("dd.MM.yyyy"));

    ui->lbGarantMon->setText(QString::number(device->garantMonth));
    ui->lbIcon->setPixmap(QPixmap(iconType));
    ui->lbName->setText(device->name);
    ui->lbNumber2->setText(device->number2);
    ui->lbType->setText(device->type.typeName);
    ui->lbVNFT->setText(device->type.VNFT);

    LoadShipping();
    number = device->number;

}

CardProdWindow::~CardProdWindow()
{
    delete ui;
}




//-------------------------------------------------------------------------------------------------------
// Загрузка статусов для выбраннного изделия или модуля
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::LoadHistoryToForm(QList<Status> &listStatus)
{
    ui->twHistory->setRowCount(0);
    int row = 0;
    for(const auto &it : listStatus)
    {
        ui->twHistory->insertRow(row);
        QTableWidgetItem *item = new QTableWidgetItem(it.dateStatus.toString("dd.MM.yyyy"));
        item->setData(Qt::UserRole, it.linkField);
        ui->twHistory->setItem(row, 0, item);

        item = new QTableWidgetItem(it.nameStatus);
        ui->twHistory->setItem(row, 1, item);

        item = new QTableWidgetItem(it.numberDoc);
        ui->twHistory->setItem(row, 2, item);

        item = new QTableWidgetItem(it.Comment);
        ui->twHistory->setItem(row, 3, item);

        ++row;
    }

    ui->twHistory->resizeColumnsToContents();
    ui->twHistory->resizeRowsToContents();

    ui->twHistory->addAction("Карточка изделия", this, [this] () {

        if(currentIdLink > 0)
        {
            Items dev = repo.GetItem(currentIdLink);
            if(dev.id > 0 && dev.type.indexType <= IndexType::Plate)
            {
                CardProdWindow *win = new CardProdWindow(&dev, this);
                win->exec();
            }
        }

    });

    ui->twHistory->addAction("Скопировать номер изделия", this, [this] () {
        if(currentIdLink > 0)
        {
            Items dev = repo.GetItem(currentIdLink);
            QClipboard *cpb = QApplication::clipboard();
            cpb->setText(dev.number, QClipboard::Clipboard);
        }
    });

}

//-------------------------------------------------------------------------------------------------------
// Загрузка ремонтов на форму экрана
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::LoadRemontToForm(int idItem)
{
    QList<Remont> listRemont;
    repo.LoadRemont(listRemont, idItem);

    ui->twRemont->setRowCount(listRemont.size());

    int row = 0;
    for(auto &it : listRemont)
    {
        Claim claim = repo.GetClaim(it.idClaim);
        QString reason = repo.GetRemontReason(it.idReason);

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(claim.number);
        ui->twRemont->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setText(it.startDate.toString("dd.MM.yyyy"));
        ui->twRemont->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setText(it.endDate.toString("dd.MM.yyyy"));
        ui->twRemont->setItem(row, 2, item);

        item = new QTableWidgetItem();
        item->setText(reason);
        ui->twRemont->setItem(row, 3, item);

        item = new QTableWidgetItem();
        item->setText(it.action);
        ui->twRemont->setItem(row, 4, item);

        item = new QTableWidgetItem();
        item->setText(it.defect);
        ui->twRemont->setItem(row, 5, item);

        item = new QTableWidgetItem();
        item->setText(it.remark);
        ui->twRemont->setItem(row, 6, item);

        ++row;
    }
}

//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::LoadShipping()
{
    if(ship.id > 0)
    {
        ui->lbDateUPD->setText(ship.dateUPD.toString("dd.MM.yyyy"));
        ui->lbNumberUPD->setText(ship.numberUPD);
        ui->lbSchet->setText(ship.schet);
        ui->lbObjectInstall->setText(ship.objectInstall);
        ui->lbNumberUPD->setText(ship.numberUPD);
        ui->lbDateUPD->setText(ship.dateUPD.toString("dd.MM.yyyy"));
        ui->lbCardOrder->setText(ship.cardOrder);
        ui->lbContract->setText(ship.dogovor);
    }
}

//-------------------------------------------------------------------------------------------------------
// Загрузка состава изделия
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::loadInclude(const Items *item)
{
    Items dev = *item;
    while(dev.idParent > 0)
        dev = repo.GetItem(dev.idParent);

    ui->treeContent->AddItem(&dev);
    ui->treeContent->AddHistoryChild();
    ui->treeContent->SetSelectItem(item->id, item->type.indexType);
}


//-------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------

void CardProdWindow::on_pbToShip_clicked()
{
    if(ship.id == 0)
        return;

    ShipWindow *win = new ShipWindow(&ship, this);
    if(win->exec() == QDialog::Accepted)
    {
        LoadShipping();
    }

}

//-------------------------------------------------------------------------------------------------------
// Кнопка редактирования типа
//-------------------------------------------------------------------------------------------------------
void CardProdWindow::on_tbChangeType_clicked()
{
    ChangeTypeDlg *win = new ChangeTypeDlg(device, this);
    if(win->exec() == QDialog::Accepted)
    {
        ui->lbType->setText(device->type.typeName);
        ui->lbVNFT->setText(device->type.VNFT);
        repo.UpdateItem(*device);
    }
}


void CardProdWindow::on_twHistory_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow != previousRow)
    {
        auto item = ui->twHistory->item(currentRow, 0);
        currentIdLink = item->data(Qt::UserRole).toInt();
        if(currentIdLink > 0)
            ui->twHistory->setContextMenuPolicy(Qt::ActionsContextMenu);
        else
            ui->twHistory->setContextMenuPolicy(Qt::NoContextMenu);
    }
}

