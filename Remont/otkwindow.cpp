#include "otkwindow.h"
#include "ui_otkwindow.h"

#include <OTKActionDlg.h>

OTKWindow::OTKWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OTKWindow)
{
    ui->setupUi(this);

    RestartLoad();
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
    model = new PlateModel(ItemType::All, this);
    model->prepareLoad2(ui->leSearch->text(), stat, false, false);
    ui->tableView->setModel(model);
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

    OtkActionDlg *win = new OtkActionDlg( item, this);
    if(win->exec() == QDialog::Accepted)
    {
        QString nameType, nameIcon;
        item->GetInfo(nameType, nameIcon);
        QIcon icon(nameIcon);

        Status stat = item->listStatus.last();
        if(stat.idStatus == Status::CORRECT)
        {
            QListWidgetItem *listItem = new QListWidgetItem(icon, item->GetDefaultName());
            listItem->setData(Qt::UserRole, item->id);
            ui->lwCheckedProd->insertItem(ui->lwBrokenProd->count(), listItem);
        }
        else if(stat.idStatus == Status::FAULTY)
        {
            QListWidgetItem *listItem = new QListWidgetItem(icon, item->GetDefaultName() + " [" + item->getLastComment() + "]");
            listItem->setData(Qt::UserRole, item->id);
            ui->lwBrokenProd->insertItem(ui->lwBrokenProd->count(), listItem);
        }
        // удалить из общего списка
        model->DeleteItemFromList(index.row());
    }
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
        Q_ASSERT(dev.listStatus.size() != 0 || dev.listStatus.last().idStatus != Status::CORRECT);
        dev.DeleteLastStatus(dev);
        delete ui->lwCheckedProd->currentItem();
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
        Q_ASSERT(dev.listStatus.size() != 0 || dev.listStatus.last().idStatus != Status::FAULTY);

        dev.DeleteLastStatus(dev);
        delete ui->lwBrokenProd->currentItem();
        model->AddItem(&dev);
    }

}

