#include "createprodwindow.h"
#include "platelistwindow.h"
#include "scan.h"
// #include "selectplatewindow.h"
#include "repofp.h"
#include "ui_createprodwindow.h"

// #include <models/listmodul.h>
// #include <models/listplate.h>
#include <models/listmodul.h>
#include <models/listplate.h>
// #include <models/modul.h>
// #include <models/product.h>

#include <QMessageBox>

CreateProductWindow::CreateProductWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateProductWindow)/*, selectIdPlate(0)*/
{

    ui->setupUi(this);

    ui->deCreateDateP->setDateTime(QDateTime::currentDateTime());

    repo.LoadTypeItem(ItemType::Product, listTypeProduct);
    for(auto &it : listTypeProduct)
        ui->cbProduct->addItem(it.VNFT + " " + it.typeName, it.id);

    ui->cbProduct->view()->setMaximumWidth(900);
    ui->cbProduct->setCurrentIndex(-1);
    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

CreateProductWindow::~CreateProductWindow()
{
    disconnect(conn);
    delete ui;
}

//---------------------------------------------------------------------------------
// Отображение количества зарегстрированных изделий по накладной
//---------------------------------------------------------------------------------
void CreateProductWindow::UpdateUseCount()
{
    if(countFromDoc > 0)
        ui->lbCountP->setText(QString("%1 (рег. %2)").arg(countFromDoc).arg(countUse));
    else
        ui->lbCountP->clear();

    if(countUse > countFromDoc)
        ui->lbCountP->setStyleSheet("border: 2px solid #FF0000;");
    else
        ui->lbCountP->setStyleSheet("border: 1px solid #000000;");

}


//---------------------------------------------------------------------------------
// Удаление изделия
//---------------------------------------------------------------------------------
void CreateProductWindow::on_tbDeleteProduct_clicked()
{
    QListWidgetItem *item = ui->lwProduct->currentItem();

    if(item == nullptr)
        return;

    int id = item->data(Qt::UserRole).toInt();

    if(repo.DeleteItem(id))
    {
        delete ui->lwProduct->currentItem();
        --countUse;
        UpdateUseCount();
    }

}

//---------------------------------------------------------------------------------
// Кнопка Регистрации изделия
//---------------------------------------------------------------------------------
void CreateProductWindow::on_pbRegProduct_clicked()
{
    if(ui->leNumProduct->text().isEmpty())
        return;

    Items prod;
    // Добавление изделия в  базу данных со статусом Создан
    prod.number = ui->leNumProduct->text();
    prod.name = ui->leNameProd->text();
    prod.idType = ui->cbProduct->currentData(Qt::UserRole).toInt();
    prod.dateCreate = QDateTime::currentDateTime();
    prod.garantMonth = listTypeProduct[ui->cbProduct->currentIndex()].garantMonth;
    prod.numberDoc = ui->leNumberDocP->text();

    if(prod.idType <= 0)
    {
        QMessageBox::critical(this, "Ошибка", "Нужно выбрать обозначение изделия (ВНФТ)", QMessageBox::Ok);
        return;
    }

    if(repo.AddItem(prod))
    {
        prod.AddStatus(prod, Status::CREATE);

        QString s = ui->cbProduct->currentText();
        QListWidgetItem *item = new QListWidgetItem(ui->leNumProduct->text() + " (" + s + ")");
        item->setData(Qt::UserRole, prod.id);
        ui->lwProduct->addItem(item);
        ui->leNumProduct->clear();
        ui->leNumProduct->setFocus();
        ++countUse;
        UpdateUseCount();

        if(ui->chComplectation->isChecked())
        {
            qDebug() << "This make complectation window.";
        }

    }
    else
        QMessageBox::warning(this, "Ошибка", QString("Изделие с серийным номером %1 уже присутствует в базе данных.").arg(prod.number));

}


//---------------------------------------------------------------------------------
// Изменение типа изделия в выпадающем списке
//---------------------------------------------------------------------------------
void CreateProductWindow::on_cbProduct_currentIndexChanged(int index)
{
    if(index < 0)
        return;

    // int key = ui->cbProduct->currentData().toInt();
    int row = ui->cbProduct->currentIndex();
    ui->lbGarantProd->setText(QString::number(listTypeProduct[row].garantMonth));
}


//---------------------------------------------------------------------------------
// Чтение сканера
//---------------------------------------------------------------------------------
void CreateProductWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        ui->leNumProduct->setText(s);
        emit ui->pbRegProduct->click();
    }
}


//---------------------------------------------------------------------------------
// поиск накладной
//---------------------------------------------------------------------------------
void CreateProductWindow::on_tbDocP_clicked()
{
    RepoFP repoFP;
    Nakl nakl;
    QList<Nakl> listNakl;
    repoFP.getDoc(ui->leNumberDocP->text(), listNakl);

    size_t size = listNakl.size();

    if(size == 0)
    {
        // накладная не найдена
    }
    else
    {
        if(size == 1)
        {
            int i = 0;
            nakl = listNakl.first();
            countFromDoc = nakl.count;
            ui->cbProduct->setCurrentIndex(-1);
            // qDebug() << nakl.VNFT << listTypeProduct.size();
            for(auto &it : listTypeProduct)
            {
                if(it.VNFT == nakl.VNFT)
                {
                    ui->cbProduct->setCurrentIndex(i);
                    countUse = repo.GetCountRegisterPlate(ui->leNumberDocP->text(), it.id);
                    break;
                }
                ++i;
            }
        }
        else
        {
            ui->cbProduct->setCurrentIndex(-1);
        }
    }

    ui->lbDocNameP->setText(nakl.name);
    ui->lbPlanP->setText(nakl.plan);
    UpdateUseCount();

}

