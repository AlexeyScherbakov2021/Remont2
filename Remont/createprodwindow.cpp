#include "createprodwindow.h"
#include "platelistwindow.h"
#include "scan.h"
#include "complectproductwindow.h"
#include "repofp.h"
#include "ui_createprodwindow.h"
#include <models/listmodul.h>
#include <models/listplate.h>
#include <QMessageBox>
#include <QCompleter>

CreateProductWindow::CreateProductWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateProductWindow)/*, selectIdPlate(0)*/
{

    ui->setupUi(this);

    ui->deCreateDateP->setDateTime(QDateTime::currentDateTime());
    model.loadList(IndexType::Product);
    proxy.setSourceModel(&model);
    proxy.sort(0);
    proxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->cbProduct->setModel(&proxy);
    ui->cbProduct->setCurrentIndex(-1);

    QCompleter *comp = new QCompleter(&proxy,ui->cbProduct);
    comp->setModel(&proxy);
    comp->setCaseSensitivity( Qt::CaseInsensitive );
    comp->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->cbProduct->setCompleter(comp);
    ui->cbProduct->setCurrentIndex(-1);
    connect(ui->cbProduct->lineEdit(), &QLineEdit::textEdited, this, &CreateProductWindow::lineEdit_textEdited);

    // ui->cbProduct->lineEdit()->completer()->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);

    // repo.LoadNewItemsType(listTypeProduct, IndexType::Product);
    // for(auto &it : listTypeProduct)
    // {
    //     QVariant var;
    //     var.setValue(&it);
    //     ui->cbProduct->addItem(it.VNFT + " " + it.typeName, var);
    // }
    // ui->cbProduct->view()->setMaximumWidth(900);



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

    if(countUse > countFromDoc && countFromDoc > 0)
        ui->lbCountP->setStyleSheet("border: 2px solid #FF0000;");
    else
        ui->lbCountP->setStyleSheet("border: 1px solid #000000;");

}


//---------------------------------------------------------------------------------
// Удаление изделия
//---------------------------------------------------------------------------------
void CreateProductWindow::on_tbDeleteProduct_clicked()
{
    int id = ui->wTree->DeleteSelectedItem();

    if(id == 0)
        return;

    if(repo.DeleteItem(id))
    {
        --countUse;
        UpdateUseCount();
    }
}

//---------------------------------------------------------------------------------
// Кнопка Регистрации изделия
//---------------------------------------------------------------------------------
void CreateProductWindow::on_pbRegProduct_clicked()
{
    // if(ui->leNumProduct->text().isEmpty())
    // if(ui->cbProduct->currentIndex() < 0)
    //     return;

    Items prod;

    if(ui->cbProduct->currentIndex() < 0)
    {
        QMessageBox::critical(this, "Ошибка", "Нужно выбрать обозначение изделия (ВНФТ)", QMessageBox::Ok);
        return;
    }

    QModelIndex index = proxy.index(ui->cbProduct->currentIndex(), 0);
    index = proxy.mapToSource(index);
    ItemType *type = model.getType(index.row());

    // qDebug() << type->typeName;
    // QVariant var = ui->cbProduct->currentData();
    // if(!var.isValid())

    // return;

    // Добавление изделия в  базу данных со статусом Создан
    // prod.type = *var.value<ItemType*>();
    prod.type = *type;
    prod.number = ui->leNumProduct->text();
    prod.name = ui->leNameProd->text();
    prod.idType = prod.type.id;
    prod.dateCreate = QDateTime::currentDateTime();
    prod.garantMonth = type->garantMonth;
    // prod.garantMonth = listTypeProduct[ui->cbProduct->currentIndex()].garantMonth;
    prod.numberDoc = ui->leNumberDocP->text();
    prod.number2 = ui->leNumber2->text();
    prod.descript = ui->leDescript->text();

    if(repo.AddItem(prod))
    {
        Status stat;
        stat.idStatus = StatusItem::CREATE;
        stat.numberDoc = ui->leNumberDocP->text();
        prod.AddStatus(prod, stat);

        // prod.AddStatus(prod, StatusItem::CREATE);

        ui->leNumProduct->clear();
        ui->leNumProduct->setFocus();
        ++countUse;
        UpdateUseCount();

        if(ui->chComplectation->isChecked())
        {
            ComplectProductWindow *win = new ComplectProductWindow(this, &prod);
            win->exec();
        }
        ui->wTree->AddItem(&prod);
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

    QModelIndex ind = proxy.mapToSource(proxy.index(index, 0));
    index = ind.row();
    ItemType *tp = model.getType(index);

    ui->lbGarantProd->setText(QString::number(tp->garantMonth));
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
            // int i = 0;
            nakl = listNakl.first();
            countFromDoc = nakl.count;

            ui->cbProduct->setCurrentIndex(-1);

            ItemType *type = model.searchVNFT(nakl.VNFT);

            if(type != nullptr)
            {
                countUse = repo.GetCountRegisterPlate(ui->leNumberDocP->text(), type->id);
                countFromDoc = nakl.count;
                proxy.setFilterFixedString(type->typeName);
                ui->cbProduct->setCurrentIndex(0);
            }
            // ui->cbProduct->setCurrentIndex(index);

            // for(auto &it : listTypeProduct)
            // {
            //     if(it.VNFT == nakl.VNFT)
            //     {
            //         ui->cbProduct->setCurrentIndex(i);
            //         countUse = repo.GetCountRegisterPlate(ui->leNumberDocP->text(), it.id);
            //         break;
            //     }
            //     ++i;
            // }
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



void CreateProductWindow::lineEdit_textEdited(const QString &arg1)
{
    // qDebug() << arg1;
    proxy.setFilterFixedString(arg1);
}





