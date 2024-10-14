#include "startworkwindow.h"
#include "ui_startworkwindow.h"

StartWorkWindow::StartWorkWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartWorkWindow)
{
    ui->setupUi(this);

    products.FindItems("", Status::SHIPPED);

    for(auto &it : products.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(it.name + " (" + it.number + ")");
        item->setData(Qt::UserRole, it.id);
        ui->lwProduct->addItem(item);
    }
    ui->deDate->setDateTime(QDateTime::currentDateTime());
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
    auto item = ui->lwProduct->currentItem();
    if(item == nullptr)
        return;

    int id = item->data(Qt::UserRole).toInt();
    Product prod = products.GetItem(id);
    // qDebug() << ui->deDate->dateTime() << prod.garantMonth;
    prod.EndGarant = ui->deDate->dateTime().addMonths(prod.garantMonth);
    // qDebug() << prod.EndGarant;
    repo.LoadChildProduct(prod);
    prod.AddStatus(prod, Status::WORK, ui->deDate->dateTime());

    for(auto &it : prod.listModules)
    {
        it.EndGarant = ui->deDate->dateTime().addMonths(it.garantMonth);
        repo.UpdateItem(it);
        it.AddStatus(it,Status::WORK, ui->deDate->dateTime());
    }

    repo.UpdateItem(prod);

    delete item;
}


