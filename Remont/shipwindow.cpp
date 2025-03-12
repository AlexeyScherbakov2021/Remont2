#include <QClipboard>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include "cardprodwindow.h"
#include "scan.h"
#include "selectdevicewindow.h"
#include "setterdlg.h"
#include "shipwindow.h"
#include "ui_shipwindow.h"
// #include <QElapsedTimer>

ShipWindow::ShipWindow(Shipment *shipment, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ShipWindow), ship(shipment)
{

    ui->setupUi(this);

    ui->deDateOut->setNullDate(QDate(1900,1,1));

    QFuture<void> future =  QtConcurrent::run( [&] (QPromise<void> &promise)
    {
        // qDebug() << "LoadOrgAsync";
        RepoMSSQL repo2("thread");
        // repo2.LoadOrganizationAsync(listOrg, promise);
        repo2.LoadOrganizationAsync(listOrg, promise);
        // qDebug() << "LoadOrgAsync finish";
    });


    watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, watcher, [this] () {
        int selectRow = -1;
        for(auto &it : listOrg)
        {
            ui->cbCusomer->addItem(it.orgName + "  (ИНН " + it.INN + " КПП" + it.KPP + ")", it.id);
            if(it.id == ship->idOrganization)
                selectRow = ui->cbCusomer->count() - 1;
        }

        ui->cbCusomer->setCurrentIndex(selectRow);
    });

    watcher->setFuture(future);

    if(ship->id != 0)
    {
        // была выбрана существующая отгрузка
        ui->leBuyer->setText(ship->buyer);
        ui->leCardOrder->setText(ship->cardOrder);
        ui->leNumUPD->setText(ship->numberUPD);
        ui->leObjectInstall->setText(ship->objectInstall);
        ui->leObjectInstall->setCursorPosition(0);
        ui->leSchet->setText(ship->schet);
        ui->leCustomer->setText(ship->customer);
        ui->deDateUPD->setDateTime(ship->dateUPD);
        ui->deDateOut->setDateTime(ship->dateRegister);

        repo.LoadChildShip(*ship);

        for(auto &it : ship->listSetterOut)
        {
            ui->wTreeItems->AddItem(&it);
        }

        for(auto &it : ship->childItems)
        {
            ui->wTreeItems->AddItem(&it);
        }
    }
    else
    {
        ui->deDateOut->setDateTime(QDateTime::currentDateTime());
    }

    if(!shipment->dateUPD.isNull())
    {
        ui->tbAddSetter->setEnabled(false);
        ui->tbNumProd->setEnabled(false);
        ui->pbDelete->setEnabled(false);
        ui->pbSave->setVisible(false);
        ui->pbFinish->setVisible(false);
    }

    connect(ui->leBuyer, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leCardOrder, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leNumUPD, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leObjectInstall, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->leSchet, SIGNAL(textChanged(QString)), SLOT(slotIsEditing()));
    connect(ui->cbCusomer, SIGNAL(currentIndexChanged(int)), SLOT(slotIsEditing()));
    connect(ui->deDateOut, SIGNAL(dateChanged(QDate)), SLOT(slotIsEditing()));

    connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

    ui->wTreeItems->addAction("Карточка устройства", this, SLOT(slotShowCard()));
    ui->wTreeItems->addAction("Скопировать номер", this, [this] () {
        auto [id, indexType] = ui->wTreeItems->GetSelectedItem();
        if(indexType <= IndexType::Plate)
        {
            Items dev = repo.GetItem(id);
            QClipboard *cpb = QApplication::clipboard();
            cpb->setText(dev.number, QClipboard::Clipboard);
            qDebug() <<  dev.number;
        }
    });


    ui->wTreeItems->setContextMenuPolicy(Qt::ActionsContextMenu);

}


ShipWindow::~ShipWindow()
{
    delete ui;
    watcher->cancel();
    watcher->waitForFinished();
    delete watcher;
    // qDebug() << "destructor ShipWindow";
}




//-----------------------------------------------------------------------------------
// Добавление продукта в отгрузку
//-----------------------------------------------------------------------------------
void ShipWindow::on_tbNumProd_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(IndexType::Product, this);
    win->AddSelectedType(IndexType::Modul);
    win->AddSelectedType(IndexType::Plate);

    Items *dev = win->SelectDevice(true, {StatusItem::CORRECT, StatusItem::CORRECT_OSO} );

    if(dev != nullptr)
    {
        // qDebug() << dev->number;
        trackItem.AddRecord(/*dev->id,*/ *dev);
        ui->wTreeItems->AddItem(dev);
    }

}



//-----------------------------------------------------------------------------------
// Удаление ветки в дереве
//-----------------------------------------------------------------------------------
void ShipWindow::on_pbDelete_clicked()
{
    IndexType type;
    int id = ui->wTreeItems->GetCurrentRootItemId(type);
    if(id <= 0)
        return;

    if(!ui->wTreeItems->DeleteSelectedItem())
        return;

    if(type == IndexType::SetterType)
    {
        for(auto &it : ship->listSetterOut)
            if(it.id == id)
            {
                trackSet.DelRecord(/*id,*/ it);
                break;
            }
    }
    else if(type <= IndexType::Plate)
    {
        for(auto &it : ship->childItems)
            if(it.id == id)
            {
                trackItem.DelRecord(/*id, */it);
                break;
            }
    }

}



//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

void ShipWindow::SetStatusItems(QList<Items> &items)
{
    QDateTime dateShip = ui->deDateUPD->dateTime();
    for(auto &dev : items)
    {
        dev.AddStatus(dev, {StatusItem::SHIPPED}, dateShip);
        dev.dateGarant = dateShip.addMonths(dev.garantMonth);
        repo.UpdateItem(dev);
        SetStatusItems(dev.childItems);
    }
}



//-----------------------------------------------------------------------------------
// Событие закрытия окна
//-----------------------------------------------------------------------------------
// void ShipWindow::on_ShipWindow_finished(int /*result*/)
// {
//     ship->buyer = ui->leBuyer->text();
//     ship->cardOrder = ui->leCardOrder->text();
//     // ship->customer = ui->leCustomer->text();
//     ship->numberUPD = ui->leNumUPD->text();
//     ship->objectInstall = ui->leObjectInstall->text();
//     ship->schet = ui->leSchet->text();
//     ship->dateUPD = ui->deDateUPD->dateTime();
//     if(ui->cbCusomer->currentIndex() >= 0)
//     {
//         ship->idOrganization = ui->cbCusomer->currentData().toInt();
//         ship->customer = ui->cbCusomer->currentText();
//     }
//     repo.UpdateItem(*ship);
// }



//-----------------------------------------------------------------------------------
// Событие изменения полей
//-----------------------------------------------------------------------------------
void ShipWindow::slotIsEditing()
{
    // qDebug() << ui->deDateOut->date();
    isEditing = true;
}


//-----------------------------------------------------------------------------------
// Кнопка добавления нового набора
//-----------------------------------------------------------------------------------
void ShipWindow::on_tbAddSetter_clicked()
{
    QScopedPointer<SetterDlg> win (new SetterDlg(true, true));
    if(win->exec() == QDialog::Accepted && win->selectSetter != nullptr)
    {
        trackSet.AddRecord(/*win->selectSetter->id,*/ *win->selectSetter);
        ui->wTreeItems->AddItem(win->selectSetter);
    }
}


//-----------------------------------------------------------------------------------
// сохранить в базу данных
//-----------------------------------------------------------------------------------
void ShipWindow::SaveToBase()
{
    ui->deDateOut->minimumDateTime();

    ship->buyer = ui->leBuyer->text();
    ship->cardOrder = ui->leCardOrder->text();
    ship->numberUPD = ui->leNumUPD->text();
    ship->objectInstall = ui->leObjectInstall->text();
    ship->schet = ui->leSchet->text();
    ship->customer = ui->leCustomer->text();
    ship->idOrganization = ui->cbCusomer->currentData().toInt();
    ship->org = repo.GetOrganization(ship->idOrganization);

    // qDebug() << ui->deDateUPD->date().year() << ui->deDateUPD->date().month();

    if(ui->deDateUPD->isNull())
        ship->dateUPD = QDateTime::fromString("00.00.0000","dd.MM.yyyy");
    else
        ship->dateUPD = ui->deDateUPD->dateTime();

    // if(ui->deDateUPD->date().year() < 1900)
    //     ship->dateUPD = QDateTime::fromString("00.00.0000","dd.MM.yyyy");
    // else
    //     ship->dateUPD = ui->deDateUPD->dateTime();

    // if(ui->deDateOut->date().year() <  1900)
    if(ui->deDateOut->isNull())
        ship->dateRegister = QDateTime::fromString("00.00.0000","dd.MM.yyyy");
    else
        ship->dateRegister = ui->deDateOut->dateTime();

    if(ship->id == 0)
        repo.AddItem(*ship);
    else
        repo.UpdateItem(*ship);

    repo.ItemsSyncShip(ship->id, &trackItem);
    repo.SetsSyncShip(ship->id, &trackSet);
}


//-----------------------------------------------------------------------------------
// Кнопка Сохранить
//-----------------------------------------------------------------------------------
void ShipWindow::on_pbSave_clicked()
{
    isEditing |= trackItem.listAdd.size() > 0 || trackItem.listDel.size() > 0
                  || trackSet.listAdd.size() > 0 || trackSet.listDel.size() > 0;

    if(!isEditing)
        reject();

    SaveToBase();
    accept();
}


//-----------------------------------------------------------------------------------
// Сканирование
//-----------------------------------------------------------------------------------
void ShipWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        RepoMSSQL repo;
        Items dev = repo.GetItem2(s, {StatusItem::CORRECT, StatusItem::CORRECT_OSO});
        if(dev.id != 0)
        {
            trackItem.AddRecord(/*dev.id,*/ dev);
            ui->wTreeItems->AddItem(&dev);
        }
    }
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void ShipWindow::slotShowCard()
{
    auto [id, indexType] = ui->wTreeItems->GetSelectedItem();
    // QPair<int, IndexType> pair = ui->wTreeItems->GetSelectedItem();
    if(id > 0 && indexType <= IndexType::Plate)
    {
        Items dev = repo.GetItem(id);
        CardProdWindow *win = new CardProdWindow(&dev, this);
        win->exec();
    }
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void ShipWindow::SyncTrack()
{
    QList<Items> list;

    trackItem.getListDel(list);
    for(auto &it : list)
    {
        ship->childItems.removeIf([&it](const Items &item) { return item.id == it.id; });
    }

    trackItem.getListAdd(list);
    ship->childItems.append(list);

    QList<SetterOut> listSet;

    trackSet.getListDel(listSet);
    for(auto &it : listSet)
    {
        ship->listSetterOut.removeIf([&it](const SetterOut &item) { return item.id == it.id; });
    }

    trackSet.getListAdd(listSet);
    ship->listSetterOut.append(listSet);

}


//-----------------------------------------------------------------------------------
// Кнопка Отгрузить
//-----------------------------------------------------------------------------------
void ShipWindow::on_pbFinish_clicked()
{
    if(ui->leNumUPD->text().isEmpty())
    {
        QMessageBox::warning(this, "Предупреждение", "Для отгрузки необходимо указать № реализации.");
        return;
    }

    SyncTrack();

    if(ship->childItems.size() == 0 && ship->listSetterOut.size() == 0)
    {
        QMessageBox::warning(this, "Предупреждение", "Не сформирован состав отгрузки.");
        return;
    }

     if(ui->deDateUPD->dateTime() == ui->deDateUPD->minimumDateTime())
        ui->deDateUPD->setDateTime(QDateTime::currentDateTime());


    // установить статус Отгружен для всех устройств
    for(auto &it : ship->listSetterOut)
    {
        SetStatusItems(it.childItems);
    }

    SetStatusItems(ship->childItems);
    SaveToBase();
    accept();
}




