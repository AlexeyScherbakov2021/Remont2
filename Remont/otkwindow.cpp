#include "otkwindow.h"
#include "scan.h"
#include "ui_otkwindow.h"

#include <OTKActionDlg.h>

OTKWindow::OTKWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OTKWindow)
{
    ui->setupUi(this);

    RestartLoad();
    connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));
}


OTKWindow::~OTKWindow()
{
    delete ui;
}


//------------------------------------------------------------------------------------------------
// Кнопка фильтр
//------------------------------------------------------------------------------------------------
void OTKWindow::on_toolButton_clicked()
{
    RestartLoad();
}

//------------------------------------------------------------------------------------------------
// Перезапрос из базы по фильтру
//------------------------------------------------------------------------------------------------
void OTKWindow::RestartLoad()
{
    delete model;
    model = new DeviceModel(IndexType::All, this);
    model->prepareLoad2(ui->leSearch->text(), stat, false, false);
    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(0, 24);
    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 150);
    ui->tableView->setColumnWidth(3, 150);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 80);
    ui->tableView->setColumnWidth(6, 100);

}

//------------------------------------------------------------------------------------------------
// Запуск окна с проверкой
//------------------------------------------------------------------------------------------------
void OTKWindow::StartControlWindow(Items *item)
{
    OtkActionDlg *win = new OtkActionDlg( item, this);
    if(win->exec() == QDialog::Accepted)
    {
        QString nameType, nameIcon;
        item->GetInfo(nameType, nameIcon);
        QIcon icon(nameIcon);

        Status stat = item->listStatus.last();
        if(stat.idStatus == StatusItem::CORRECT)
        {
            QListWidgetItem *listItem = new QListWidgetItem(icon, item->GetDefaultName());
            listItem->setData(Qt::UserRole, item->id);
            ui->lwCheckedProd->insertItem(ui->lwBrokenProd->count(), listItem);
        }
        else if(stat.idStatus == StatusItem::FAULTY)
        {
            QListWidgetItem *listItem = new QListWidgetItem(icon, item->GetDefaultName() + " [" + item->getLastComment() + "]");
            listItem->setData(Qt::UserRole, item->id);
            ui->lwBrokenProd->insertItem(ui->lwBrokenProd->count(), listItem);
        }

        // удалить из общего списка
        model->DeleteItemFromListId(item->id);
    }
}


//------------------------------------------------------------------------------------------------
// Кнопка выбрать для контроля
//------------------------------------------------------------------------------------------------
void OTKWindow::on_pbSelect_clicked()
{
    on_tableView_doubleClicked(ui->tableView->currentIndex());
}


//------------------------------------------------------------------------------------------------
// Двойной щелчок выбрать для контроля
//------------------------------------------------------------------------------------------------
void OTKWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(index == QModelIndex())
        return;

    Items *item = model->GetItem(index.row());
    StartControlWindow(item);
}


//------------------------------------------------------------------------------------------------
// Отменить прошедшую проверку устройства
//------------------------------------------------------------------------------------------------
void OTKWindow::on_tbDelCheckProd_clicked()
{
    RepoMSSQL repo;

    QListWidgetItem *listItem = ui->lwCheckedProd->currentItem();
    int id = listItem->data(Qt::UserRole).toInt();
    Items dev = repo.GetItem(id);
    if(dev.id > 0)
    {
        Q_ASSERT(dev.listStatus.size() != 0 || dev.listStatus.last().idStatus != StatusItem::CORRECT);
        dev.DeleteLastStatus(dev, StatusItem::CORRECT);
        delete ui->lwCheckedProd->currentItem();
        repo.LoadStatus(dev);
        model->AddItem(&dev);

    }
}


//------------------------------------------------------------------------------------------------
// Отменить непрошедшую проверку устройства
//------------------------------------------------------------------------------------------------
void OTKWindow::on_tbDelBrokenProd_clicked()
{
    RepoMSSQL repo;

    QListWidgetItem *listItem = ui->lwBrokenProd->currentItem();
    int id = listItem->data(Qt::UserRole).toInt();
    Items dev = repo.GetItem(id);
    if(dev.id > 0)
    {
        Q_ASSERT(dev.listStatus.size() != 0 || dev.listStatus.last().idStatus != StatusItem::FAULTY);

        dev.DeleteLastStatus(dev, StatusItem::FAULTY);
        delete ui->lwBrokenProd->currentItem();
        repo.LoadStatus(dev);
        model->AddItem(&dev);
    }
}

//------------------------------------------------------------------------------------------------
// Событие сканера
//------------------------------------------------------------------------------------------------
void OTKWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        RepoMSSQL repo;
        Items item = repo.GetItem2(s, stat);

        if(item.id != 0)
            StartControlWindow(&item);
    }
}

