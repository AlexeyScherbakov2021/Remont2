#include "startworkwindow.h"
#include "ui_startworkwindow.h"

StartWorkWindow::StartWorkWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartWorkWindow)
{
    ui->setupUi(this);

    products.FindItems(Status::SHIPPED);
    modules.FindItems(Status::SHIPPED);
    // repo.LoadProducts(listProduct, Status::Stat::SHIPPED);
    // repo.LoadModulsStatus(listModul, Status::Stat::SHIPPED);

    for(auto &it : products.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it.name + " (" + it.number + ")");
        item->setData(Qt::UserRole, it.id);
        ui->lwProduct->addItem(item);
    }

    for(auto &it : modules.listItems)
    {
        if(it.idProduct > 0)
            continue;
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it.name + " (" + it.number + ")");
        item->setData(Qt::UserRole, it.id);
        ui->lwModul->addItem(item);
    }
    ui->deDate->setDateTime(QDateTime::currentDateTime());
}

StartWorkWindow::~StartWorkWindow()
{
    delete ui;
}

void StartWorkWindow::on_pbProdToWork_clicked()
{
    auto item = ui->lwProduct->currentItem();
    if(item == nullptr)
        return;

    Product prod;
    prod.id = item->data(Qt::UserRole).toInt();

    Status status;
    status.idStatus = Status::Stat::WORK;
    status.idDevice = prod.id;
    status.dateStatus = ui->deDate->dateTime();

    prod.AddStatus(prod, status);
    // repo.AddStatusProduct(status);

    delete item;
}


void StartWorkWindow::on_pbModulToWork_clicked()
{
    auto item = ui->lwModul->currentItem();
    if(item == nullptr)
        return;

    Modul mod;
    mod.id = item->data(Qt::UserRole).toInt();

    Status status;
    status.idStatus = Status::Stat::WORK;
    status.idDevice = mod.id;
    status.dateStatus = ui->deDate->dateTime();

    mod.AddStatus(mod, status);
    // repo.AddStatusModul(status);

    delete item;

}

