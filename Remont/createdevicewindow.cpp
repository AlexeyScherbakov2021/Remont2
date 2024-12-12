#include "createdevicewindow.h"
#include "platelistwindow.h"
#include "scan.h"
// #include "selectplatewindow.h"
#include "repofp.h"
#include "ui_createdevicewindow.h"

// #include <models/listmodul.h>
// #include <models/listplate.h>
#include <models/listmodul.h>
#include <models/listplate.h>
#include <models/modul.h>
#include <models/product.h>

#include <QMessageBox>

CreateDeviceWindow::CreateDeviceWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateDeviceWindow)/*, selectIdPlate(0)*/
{

    ui->setupUi(this);

    ui->deCreateDate->setDateTime(QDateTime::currentDateTime());
    ui->deCreateDateP->setDateTime(QDateTime::currentDateTime());

    repo.LoadModuleType(listTypeModule);
    for(auto &it : listTypeModule)
        ui->cbModul->addItem(it.name, it.id);


    repo.LoadProductType(listTypeProduct);
    for(auto &it : listTypeModule)
        ui->cbProduct->addItem(it.name, it.id);

    // for(auto it = listTypeModule.cbegin(); it != listTypeModule.cend(); ++it)
    //     ui->cbModul->addItem((*it).name, it.key());

    // for(auto it = listTypeProduct.cbegin(); it != listTypeProduct.cend(); ++it)
    //     ui->cbProduct->addItem((*it).name, it.key());

    // ui->cbModul->setCurrentIndex(-1);
    // ui->cbProduct->setCurrentIndex(-1);

    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

CreateDeviceWindow::~CreateDeviceWindow()
{
    disconnect(conn);
    delete ui;
}



//---------------------------------------------------------------------------------
// Удаление модуля
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_tbDeleteModul_clicked()
{
    QTreeWidgetItem *item = ui->twModul->currentItem();
    if(item->parent() != nullptr)
        item = item->parent();

    // Modul mod;
    int id = item->data(0, Qt::UserRole).toInt();
    if(repo.DeleteModul(id))
        delete item;
}


//---------------------------------------------------------------------------------
// Выбор платы по серийному номеру
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_tbSearchPlate_clicked()
{
    QString s = ui->leSearchPlate->text();

    PlateListWindow *selPlate = new PlateListWindow(this);
    selPlate->setSelect();
    selPlate->setNotLinked();
    selPlate->RemoveListPlate(listAddingPlate);
    selPlate->SelectPlate(s);

    for(auto &it : selPlate->selectedPlates)
    {
        AddPlateToScreen(it);
    }

    // Plate plate = selPlate->SelectPlate(s);

    // if(plate.id > 0)
    //     AddPlateToScreen(plate);

}


void CreateDeviceWindow::AddPlateToScreen( Plate &plate)
{
    if( std::find_if(listAddingPlate.cbegin(), listAddingPlate.cend(), [&plate]( const Plate &p){ return plate.id == p.id; }) == listAddingPlate.cend())
    {
        listAddingPlate.push_back(plate);
        addLinePlate(&plate);
        ui->twPlates->resizeColumnsToContents();
        ui->twPlates->resizeRowsToContents();
    }
}

void CreateDeviceWindow::UpdateUseCount()
{
    if(countFromDoc > 0)
        ui->lbCount->setText(QString("%1 (рег. %2)").arg(countFromDoc).arg(countUse));
    else
        ui->lbCount->clear();

    if(countUse > countFromDoc)
        ui->lbCount->setStyleSheet("border: 2px solid #FF0000;");
    else
        ui->lbCount->setStyleSheet("border: 1px solid #000000;");

}

void CreateDeviceWindow::UpdateUseCountP()
{
    if(countFromDocP > 0)
        ui->lbCountP->setText(QString("%1 (рег. %2)").arg(countFromDocP).arg(countUseP));
    else
        ui->lbCountP->clear();

    if(countUseP > countFromDocP)
        ui->lbCountP->setStyleSheet("border: 2px solid #FF0000;");
    else
        ui->lbCountP->setStyleSheet("border: 1px solid #000000;");

}


//---------------------------------------------------------------------------------
// Удаление изделия
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_tbDeleteProduct_clicked()
{
    if(ui->lwProduct->currentItem() == nullptr)
        return;

    // Product prod;
    int id = ui->lwProduct->currentItem()->data(Qt::UserRole).toInt();
    if(repo.DeleteProduct(id) )
        delete ui->lwProduct->currentItem();
}



//---------------------------------------------------------------------------------
// Добавление строки в список плат
//---------------------------------------------------------------------------------
void CreateDeviceWindow::addLinePlate(Plate *plate)
{
    int row = ui->twPlates->rowCount();
    ui->twPlates->insertRow(row);

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(plate->number);
    item->setData(Qt::UserRole, plate->id);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 0, item);

    item = new QTableWidgetItem();
    item->setText(plate->number2);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 1, item);

    item = new QTableWidgetItem();
    item->setText(plate->numberDoc);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 2, item);

    item = new QTableWidgetItem();
    item->setText(plate->VNFT);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 3, item);

    item = new QTableWidgetItem();
    item->setText(plate->dateRegister.toString("dd.MM.yyyy"));
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->twPlates->setItem(row, 4, item);

}



//---------------------------------------------------------------------------------
// Добавление строки в список модулей
//---------------------------------------------------------------------------------
void CreateDeviceWindow::addLineModul(Modul &mod)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, mod.number);
    item->setData(0, Qt::UserRole, mod.id);
    ui->twModul->addTopLevelItem(item);

    for(const auto &it : mod.listPlate)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(0, it.number + " (FW: " + it.number2 + ")");
        item->addChild(child);
    }
    item->setExpanded(true);

}


//---------------------------------------------------------------------------------
// Кнопка Регистрации изделия
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_pbRegProduct_clicked()
{
    if(ui->leNumProduct->text().isEmpty())
        return;

    Product prod;
    // Добавление изделия в  базу данных со статусом Создан
    prod.number = ui->leNumProduct->text();
    prod.name = ui->leNameProd->text();
    prod.idType = ui->cbProduct->currentData(Qt::UserRole).toInt();
    prod.dateRegister = QDateTime::currentDateTime();
    prod.garantMonth = listTypeProduct[prod.idType].garantMonth;
    if(repo.AddItem(prod))
    {
        // Status status;
        // status.idDevice = prod.id;
        // status.idStatus = Status::CREATE;
        // status.dateStatus = QDateTime::currentDateTime();
        prod.AddStatus(prod, Status::CREATE);
        // repo.AddStatusProduct(status);

        QString s = ui->cbProduct->currentText();
        QListWidgetItem *item = new QListWidgetItem(ui->leNumProduct->text() + " (" + s + ")");
        item->setData(Qt::UserRole, prod.id);
        ui->lwProduct->addItem(item);
        ui->leNumProduct->clear();
        ui->leNumProduct->setFocus();
    }
    else
        QMessageBox::warning(this, "Ошибка", QString("Изделие с серийным номером %1 уже присутствует в базе данных.").arg(prod.number));

}


//---------------------------------------------------------------------------------
// Кнопка Регистрации модуля
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_pbRegModul_clicked()
{
    if(ui->leNumModul->text().isEmpty())
        return;

    ListModul lModul;
    Modul mod;
    // добавить устройство в базу со статусом Создан
    mod.number = ui->leNumModul->text();
    mod.name = ui->leModulName->text();
    mod.idType = ui->cbModul->currentData(Qt::UserRole).toInt();
    // mod.numberFW = ui->lbNumberFWPlate->text();
    mod.dateRegister = QDateTime::currentDateTime();
    mod.listPlate = listAddingPlate;
    mod.garantMonth = listTypeModule[mod.idType].garantMonth;
    if(repo.AddItem(mod))
    {
        mod.AddStatus(mod, Status::CREATE);
        for(auto &it : mod.listPlate)
            lModul.LinkPlate(it.id, mod.id);

        addLineModul(mod);
        ui->leNumModul->clear();
        ui->leNumModul->setFocus();

        listAddingPlate.clear();
        ui->twPlates->clearContents();
        ui->twPlates->setRowCount(0);
    }
    else
        QMessageBox::warning(this, "Ошибка", QString("Модуль с серийным номером %1 уже присутствует в базе данных.").arg(mod.number));

}


//---------------------------------------------------------------------------------
// Кнопка Удалить плату
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_tbDelPlate_clicked()
{
    int row = ui->twPlates->currentRow();
    if(row < 0)
        return;

    listAddingPlate.removeAt(row);
    ui->twPlates->removeRow(row);
}



//---------------------------------------------------------------------------------
// Изменение типа изделия в выпадающем списке
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_cbProduct_currentIndexChanged(int index)
{
    if(index < 0)
        return;

    int key = ui->cbProduct->currentData().toInt();
    ui->lbGarantProd->setText(QString::number(listTypeProduct[key].garantMonth));

}

//---------------------------------------------------------------------------------
// Изменение типа модуля в выпадающем списке
//---------------------------------------------------------------------------------
void CreateDeviceWindow::on_cbModul_currentIndexChanged(int index)
{
    if(index < 0)
        return;

    int key = ui->cbModul->currentData().toInt();
    ui->lbGarantMod->setText(QString::number(listTypeModule[key].garantMonth));
}

//---------------------------------------------------------------------------------
// Чтение сканера
//---------------------------------------------------------------------------------
void CreateDeviceWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        if(ui->tabWidget->currentIndex() == 0)
        {
            ui->leNumProduct->setText(s);
            emit ui->pbRegProduct->click();
        }
        else
        {
            // выполнить поиск свободных плат
            Plate plate = repo.GetPlate(s);
            if(plate.id > 0 )
            {
                AddPlateToScreen(plate);
                return;
            }
            ui->leNumModul->setText(s);
            emit ui->pbRegModul->click();
        }
    }
}


void CreateDeviceWindow::on_tbDoc_clicked()
{
    RepoFP repoFP;
    Nakl nakl;
    QList<Nakl> listNakl;
    repoFP.getDoc(ui->leNumberDoc->text(), listNakl);

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
            ui->cbModul->setCurrentIndex(-1);
            for(auto &it : listTypeModule)
            {
                if(it.name == nakl.VNFT)
                {
                    ui->cbModul->setCurrentIndex(i);
                    countUse = repo.GetCountRegisterModul(ui->leNumberDoc->text(), it.id);
                    break;
                }
                ++i;
            }
        }
        else
        {
            ui->cbModul->setCurrentIndex(-1);
        }
    }

    ui->lbDocName->setText(nakl.name);
    ui->lbPlan->setText(nakl.plan);
    UpdateUseCount();

}


void CreateDeviceWindow::on_tbDocP_clicked()
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
            countFromDocP = nakl.count;
            ui->cbProduct->setCurrentIndex(-1);
            for(auto &it : listTypeProduct)
            {
                if(it.name == nakl.VNFT)
                {
                    ui->cbProduct->setCurrentIndex(i);
                    countUse = repo.GetCountRegisterProduct(ui->leNumberDocP->text(), it.id);
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
    UpdateUseCountP();

}

