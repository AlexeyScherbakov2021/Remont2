#include "startworkwindow.h"
#include "ui_startworkwindow.h"

#include <QMessageBox>

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

    if(ui->leDoc->text().isEmpty())
    {
        QMessageBox::information(this, "Сообщение", "Укажите документ.");
        return;
    }


    int id = item->data(Qt::UserRole).toInt();
    Product prod = products.GetItem(id);
    // qDebug() << ui->deDate->dateTime() << prod.garantMonth;
    prod.dateOn = ui->deDate->dateTime();
    prod.EndGarant = prod.dateOn.addMonths(prod.garantMonth);
    // qDebug() << prod.EndGarant;
    repo.LoadChildProduct(prod);
    prod.AddStatus(prod, Status::WORK, ui->deDate->dateTime(), ui->leDoc->text());

    for(auto &it : prod.listModules)
    {
        it.dateOn = ui->deDate->dateTime();
        it.EndGarant = it.dateOn.addMonths(it.garantMonth);
        repo.UpdateItem(it);
        it.AddStatus(it,Status::WORK, ui->deDate->dateTime(), ui->leDoc->text());
    }

    repo.UpdateItem(prod);

    delete item;
}


