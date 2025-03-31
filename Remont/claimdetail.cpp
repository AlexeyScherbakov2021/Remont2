#include "cardprodwindow.h"
#include "claimdetail.h"
#include "selectdevicewindow.h"
#include "ui_claimdetail.h"
#include <qmessagebox.h>
#include <models/organization.h>
#include <QClipboard>
#include <QtConcurrent>

ClaimDetail::ClaimDetail(Claim *cl, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClaimDetail), claim(cl)
{
    ui->setupUi(this);

    repo.LoadClaimType(listTypeClaim);
    for(auto it = listTypeClaim.cbegin(); it != listTypeClaim.cend(); ++it)
        ui->cbTypeClaim->addItem(*it, it.key());

    repo.LoadChildClaim(*claim);
    ClaimToScreen();

    ui->tableWidget->setColumnWidth(0, 30);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 400);
    ui->tableWidget->setColumnWidth(3, 70);

    if(cl->dateCreate.isNull())
        ui->deDateClaim->setDateTime(QDateTime::currentDateTime());

    connect(ui->leNumber, SIGNAL(editingFinished()), SLOT(slotEnabledWidget()));
    connect(ui->tableWidget, SIGNAL(currentCellChanged(int,int,int,int)), SLOT(slotEnabledWidget()));

    ui->tableWidget->addAction("Карточка устройства", this, SLOT(slotShowCard()));
    ui->tableWidget->addAction("Скопировать номер", this, [this] () {
        auto item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
        int id = item->data(Qt::UserRole).toInt();
        // if(indexType <= IndexType::Plate)
        // {
            Items dev = repo.GetItem(id);
            QClipboard *cpb = QApplication::clipboard();
            cpb->setText(dev.number, QClipboard::Clipboard);
            // qDebug() <<  dev.number;
        // }
    });

    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    slotEnabledWidget();
}

ClaimDetail::~ClaimDetail()
{
    delete ui;
    delete watcher;
}


//-----------------------------------------------------------------------------------------
// Кнопка ОК
//-----------------------------------------------------------------------------------------
void ClaimDetail::on_pbOK_clicked()
{
    claim->number = ui->leNumber->text();

    if(ui->deDateClaim->isNull())
        claim->dateCreate = QDateTime::fromString("00.00.0000","dd.MM.yyyy");
    else
        claim->dateCreate = ui->deDateClaim->dateTime();
    claim->ObjectInstall = ui->leObjectInst->text();
    claim->idTypeClaim = ui->cbTypeClaim->currentData(Qt::UserRole).toInt();
    int orgIndex = ui->cbOrg->currentData().toInt();
    claim->idOrg = orgIndex;
    claim->nameOrganization = ui->cbOrg->currentText();
    claim->TypeClaimString = ui->cbTypeClaim->currentText();

    if(claim->id == 0)
        repo.AddItem(*claim);
    else
        repo.UpdateItem(*claim);

    // Добавленные изделия
    QList<Items> listAddProduct;
    trackProduct.getListAdd(listAddProduct);
    for(auto it : listAddProduct)
    {
        if(repo.AddItemToClaim(it.id, claim->id))
        {
            it.AddStatus(it, StatusItem::FAULTY_ON_OBJECT);

            // установка статуса для родителей
            Items dev = it;
            while(dev.idParent > 0)
            {
                dev = repo.GetItem(dev.idParent);
                if(dev.listStatus.last().typeStatus != TypeStatus::REMONT_STATUS)
                    dev.AddStatus(dev, StatusItem::FAULTY_CHILD);
            }
        }
    }

    // Удаленные изделия
    QList<Items> listDelProduct;
    trackProduct.getListDel(listDelProduct);
    for(auto it : listDelProduct)
    {
        if(repo.DelItemFromClaim(it.id, claim->id))
        {
            it.DeleteLastStatus(it, StatusItem::FAULTY_ON_OBJECT);

            // удаление статуса для родителей
            Items dev = it;
            while(dev.idParent > 0)
            {
                dev = repo.GetItem(dev.idParent);
                dev.DeleteLastStatus(dev, StatusItem::FAULTY_ON_OBJECT);
            }
        }
    }

    accept();
}

//-----------------------------------------------------------------------------------------
// Отображение данных на экран
//-----------------------------------------------------------------------------------------
void ClaimDetail::ClaimToScreen(/*Claim *claim*/)
{
    ui->leNumber->setText(claim->number);
    ui->deDateClaim->setDateTime(claim->dateCreate);
    ui->leObjectInst->setText(claim->ObjectInstall);
    ui->cbTypeClaim->setCurrentText(listTypeClaim[claim->idTypeClaim]);

    // repo.LoadOrganization(listOrg);

    QFuture<void> future =  QtConcurrent::run( [&] (QPromise<void> &promise)
        {
            RepoMSSQL repo2("thread");
            repo2.LoadOrganizationAsync(listOrg, promise);
             // repo2.LoadOrganization(listOrg);
        });

    watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, watcher, [this] () {
        int selectRow = -1;
        for(auto &it : listOrg)
        {
            ui->cbOrg->addItem(it.orgName + "  (ИНН " + it.INN + " КПП" + it.KPP + ")", it.id);
            if(it.id == claim->idOrg)
                selectRow = ui->cbOrg->count() - 1;
        }

        ui->cbOrg->setCurrentIndex(selectRow);
    });

    watcher->setFuture(future);

    for(auto &it : claim->childItems)
        AddProductToTableScreen(&it);
}



//-----------------------------------------------------------------------------------------
// Добавление изделия в таблицу экрана
//-----------------------------------------------------------------------------------------
void ClaimDetail::AddProductToTableScreen(const Items *prod)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QString typeName, iconName;
    prod->GetInfo(typeName, iconName);

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setData(Qt::UserRole, prod->id);
    // item->setData(Qt::UserRole + 1, prod->type.indexType);
    item->setToolTip(typeName);
    item->setIcon(QIcon(iconName));
    ui->tableWidget->setItem(row, 0, item);

    item = new QTableWidgetItem();
    item->setText(prod->number);
    ui->tableWidget->setItem(row, 1, item);

    item = new QTableWidgetItem();
    item->setText(prod->type.typeName + " " + prod->type.VNFT);
    ui->tableWidget->setItem(row, 2, item);

    item = new QTableWidgetItem();
    item->setText(prod->dateGarant.toString("dd.MM.yyyy"));
    ui->tableWidget->setItem(row, 3, item);

}


//-----------------------------------------------------------------------------------------
// Кнопка добавления устройства
//-----------------------------------------------------------------------------------------
void ClaimDetail::on_tbAddDevice_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    Items *dev = win->SelectDevice(true, {StatusItem::WORK, StatusItem::SHIPPED}, "", true, LoadPartType::HAS_PARENT);
    if(dev != nullptr && dev->id > 0)
    {
        // проверка на вхождение в незакрытую рекламацию
        Claim claim = repo.GetClaimForItem(dev->id);
        if(claim.id > 0)
        {
            QMessageBox::warning(this, "Предупреждение", QString("Устройство № %1 находится в незакрытой рекламации № %2").arg(dev->number).arg(claim.number), QMessageBox::Ok);
            return;
        }

        AddProductToTableScreen(dev);
        trackProduct.AddRecord(*dev);
    }
}

//-----------------------------------------------------------------------------------------
// Кнопка удаления устройства
//-----------------------------------------------------------------------------------------
void ClaimDetail::on_tbDeleteDevice_clicked()
{
    QTableWidgetItem *item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    if(item == nullptr)
        return;

    int id = item->data(Qt::UserRole).toInt();
    Items dev;
    for(auto &it : claim->childItems)
    {
        if(it.id == id)
        {
            Status stat = it.listStatus.last();
            if(stat.idStatus != StatusItem::FAULTY_ON_OBJECT)
                return;
            dev = it;
            break;
        }
    }

    trackProduct.DelRecord(/*id,*/ dev);
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}


//-----------------------------------------------------------------------------------------
// Включние-выключение кнопок
//-----------------------------------------------------------------------------------------
void ClaimDetail::slotEnabledWidget()
{
    bool res = !ui->leNumber->text().isEmpty();
    ui->pbOK->setEnabled(res);

    res = (ui->tableWidget->currentRow() >= 0);
    auto item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);

    res = true;
    if(item != nullptr)
    {
        // int id = item->data(Qt::UserRole).toInt();
        // int type = item->data(Qt::UserRole + 1).toInt();

        // if(type == ev::PRODUCT)
        // {
        //     auto prod_iter = std::find_if(claim->listProduct.cbegin(), claim->listProduct.cend(), [id] (Items p) { return p.id == id;});
        //     res = (prod_iter != claim->listProduct.cend() && !(*prod_iter).getIsRepair());
        // }
        // else
        // {
        //     auto mod_iter = std::find_if(claim->listModul.cbegin(), claim->listModul.cend(), [id] (Items m) { return m.id == id;});
        //     if(mod_iter != claim->listModul.cend())
        //         res = !(*mod_iter).getIsRepair();
        // }
    }
    ui->tbDeleteDevice->setEnabled(res);
}

//-----------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------
void ClaimDetail::slotShowCard()
{
    auto item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    int id = item->data(Qt::UserRole).toInt();
    // IndexType indexType = item->data(Qt::UserRole + 1).toInt();

    if(id > 0 /*&& indexType <= IndexType::Plate*/)
    {
        Items dev = repo.GetItem(id);
        CardProdWindow *win = new CardProdWindow(&dev, this);
        win->exec();
    }
}

