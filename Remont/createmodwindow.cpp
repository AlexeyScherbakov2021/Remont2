#include "createmodwindow.h"
#include "platelistwindow.h"
#include "scan.h"
// #include "selectplatewindow.h"
#include "repofp.h"
#include "ui_createmodwindow.h"

// #include <models/listmodul.h>
// #include <models/listplate.h>
#include <models/listmodul.h>
#include <models/listplate.h>
// #include <models/modul.h>
// #include <models/product.h>

#include <QMessageBox>

CreateModulWindow::CreateModulWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateModulWindow)/*, selectIdPlate(0)*/
{

    ui->setupUi(this);

    ui->deCreateDate->setDateTime(QDateTime::currentDateTime());

    repo.LoadTypeItem(ItemType::Modul, listTypeModule);

    for(const auto &it : listTypeModule)
        ui->cbModul->addItem(it.VNFT + " " + it.typeName, it.id);

    ui->cbModul->view()->setMaximumWidth(900);
    ui->cbModul->setCurrentIndex(-1);

    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

CreateModulWindow::~CreateModulWindow()
{
    disconnect(conn);
    delete ui;
}



//---------------------------------------------------------------------------------
// Удаление модуля
//---------------------------------------------------------------------------------
void CreateModulWindow::on_tbDeleteModul_clicked()
{
    QTreeWidgetItem *item = ui->twModul->currentItem();
    if(item->parent() != nullptr)
        item = item->parent();

    // Modul mod;
    int id = item->data(0, Qt::UserRole).toInt();

    if(repo.DeleteItem(id))
    {
        delete ui->twModul->currentItem();
        --countUse;
        UpdateUseCount();
    }

}

//---------------------------------------------------------------------------------
// Отображение количества зарегстрированных изделий по накладной
//---------------------------------------------------------------------------------
void CreateModulWindow::UpdateUseCount()
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


//---------------------------------------------------------------------------------
// Добавление строки в список модулей
//---------------------------------------------------------------------------------
void CreateModulWindow::addLineModul(Items &mod)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, mod.number);
    item->setData(0, Qt::UserRole, mod.id);
    ui->twModul->addTopLevelItem(item);
    item->setExpanded(true);
}


//---------------------------------------------------------------------------------
// Кнопка Регистрации модуля
//---------------------------------------------------------------------------------
void CreateModulWindow::on_pbRegModul_clicked()
{
    if(ui->leNumModul->text().isEmpty())
        return;

    ListModul lModul;
    Items mod;
    // добавить устройство в базу со статусом Создан
    mod.number = ui->leNumModul->text();
    mod.name = ui->leModulName->text();
    mod.idType = ui->cbModul->currentData(Qt::UserRole).toInt();
    mod.dateCreate = QDateTime::currentDateTime();
    mod.garantMonth = listTypeModule[ui->cbModul->currentIndex()].garantMonth;
    mod.numberDoc = ui->leNumberDoc->text();

    if(mod.idType <= 0)
    {
        QMessageBox::critical(this, "Ошибка", "Нужно выбрать обозначение модуля (ВНФТ)", QMessageBox::Ok);
        return;
    }

    if(repo.AddItem(mod))
    {
        mod.AddStatus(mod, Status::CREATE);

        addLineModul(mod);

        ui->leNumModul->clear();
        ui->leNumModul->setFocus();

        ++countUse;
        UpdateUseCount();

        if(ui->chComplectation->isChecked())
        {
            qDebug() << "This make complectation window.";
        }

    }
    else
        QMessageBox::warning(this, "Ошибка", QString("Модуль с серийным номером %1 уже присутствует в базе данных.").arg(mod.number));

}

//---------------------------------------------------------------------------------
// Изменение типа модуля в выпадающем списке
//---------------------------------------------------------------------------------
void CreateModulWindow::on_cbModul_currentIndexChanged(int index)
{
    if(index < 0)
        return;

    // int key = ui->cbModul->currentData().toInt();

    int row = ui->cbModul->currentIndex();
    ui->lbGarantMod->setText(QString::number(listTypeModule[row].garantMonth));
}

//---------------------------------------------------------------------------------
// Чтение сканера
//---------------------------------------------------------------------------------
void CreateModulWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        ui->leNumModul->setText(s);
        emit ui->pbRegModul->click();
    }
}


//---------------------------------------------------------------------------------
// поиск накладной
//---------------------------------------------------------------------------------
void CreateModulWindow::on_tbDoc_clicked()
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
                if(it.VNFT == nakl.VNFT)
                {
                    ui->cbModul->setCurrentIndex(i);
                    countUse = repo.GetCountRegisterPlate(ui->leNumberDoc->text(), it.id);
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

