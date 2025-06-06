#include "createmodwindow.h"
#include "platelistwindow.h"
#include "scan.h"
#include "complectproductwindow.h"
#include "repofp.h"
#include "ui_createmodwindow.h"
#include <models/listmodul.h>
#include <models/listplate.h>
#include <QCompleter>
#include <QMessageBox>

CreateModulWindow::CreateModulWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateModulWindow)/*, selectIdPlate(0)*/
{

    ui->setupUi(this);

    ui->deCreateDate->setDateTime(QDateTime::currentDateTime());

    model.loadList(IndexType::Modul);
    proxy.setSourceModel(&model);
    proxy.sort(0);
    proxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->cbModul->setModel(&proxy);
    ui->cbModul->setCurrentIndex(-1);
    ui->cbModul->lineEdit()->completer()->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);


    // repo.LoadNewItemsType(listTypeModule, IndexType::Modul );

    // for(auto &it : listTypeModule)
    // {
    //     QVariant var;
    //     var.setValue(&it);
    //     ui->cbModul->addItem(it.VNFT + " " + it.typeName, var);
    // }

    // ui->cbModul->view()->setMaximumWidth(900);
    // ui->cbModul->setCurrentIndex(-1);
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
// Отображение количества зарегстрированных изделий по накладной
//---------------------------------------------------------------------------------
void CreateModulWindow::UpdateUseCount()
{
    if(countFromDoc > 0)
        ui->lbCount->setText(QString("%1 (рег. %2)").arg(countFromDoc).arg(countUse));
    else
        ui->lbCount->clear();

    // qDebug() << "countUse:" << countUse << "countFromDoc:" << countFromDoc;

    if(countUse > countFromDoc && countFromDoc > 0)
        ui->lbCount->setStyleSheet("border: 2px solid #FF0000;");
    else
        ui->lbCount->setStyleSheet("border: 1px solid #000000;");

}

//---------------------------------------------------------------------------------
// Кнопка Регистрации модуля
//---------------------------------------------------------------------------------
void CreateModulWindow::on_pbRegModul_clicked()
{
    if(ui->leNumModul->text().isEmpty())
        return;

    // ListModul lModul;
    Items mod;
    // добавить устройство в базу со статусом Создан
    // QVariant var = ui->cbModul->currentData();
    // if(!var.isValid())
    if(ui->cbModul->currentIndex() < 0)
    {
        QMessageBox::critical(this, "Ошибка", "Нужно выбрать обозначение модуля (ВНФТ)", QMessageBox::Ok);
        return;
    }

    QModelIndex index = proxy.index(ui->cbModul->currentIndex(), 0);
    index = proxy.mapToSource(index);
    ItemType *type = model.getType(index.row());


    mod.type = *type;
    mod.number = ui->leNumModul->text();
    mod.name = ui->leModulName->text();
    mod.idType = mod.type.id;
    mod.dateCreate = QDateTime::currentDateTime();
    mod.garantMonth = type->garantMonth;
    mod.numberDoc = ui->leNumberDoc->text();
    mod.number2 = ui->leNumber2->text();
    mod.descript = ui->leDescript->text();

    if(repo.AddItem(mod))
    {
        Status stat;
        stat.idStatus = StatusItem::CREATE;
        stat.numberDoc = ui->leNumberDoc->text();
        mod.AddStatus(mod, stat);

        ui->leNumModul->clear();
        ui->leNumModul->setFocus();

        ++countUse;
        UpdateUseCount();

        if(ui->chComplectation->isChecked())
        {
            // qDebug() << "This make complectation window.";
            ComplectProductWindow *win = new ComplectProductWindow(this, &mod);
            win->exec();
        }

        ui->wTree->AddItem(&mod);

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

    // QVariant var = ui->cbModul->currentData();
    // ItemType *tp = var.value<ItemType*>();

    QModelIndex ind = proxy.mapToSource(proxy.index(index, 0));
    index = ind.row();
    ItemType *tp = model.getType(index);

    ui->lbGarantMod->setText(QString::number(tp->garantMonth));
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
            // int i = 0;
            nakl = listNakl.first();
            countFromDoc = nakl.count;
            ui->cbModul->setCurrentIndex(-1);
            ItemType *type = model.searchVNFT(nakl.VNFT);

            if(type != nullptr)
            {
                countUse = repo.GetCountRegisterPlate(ui->leNumberDoc->text(), type->id);
                countFromDoc = nakl.count;
                proxy.setFilterFixedString(type->typeName);
                ui->cbModul->setCurrentIndex(0);
            }


            // for(auto &it : listTypeModule)
            // {
            //     if(it.VNFT == nakl.VNFT)
            //     {
            //         ui->cbModul->setCurrentIndex(i);
            //         countUse = repo.GetCountRegisterPlate(ui->leNumberDoc->text(), it.id);
            //         break;
            //     }
            //     ++i;
            // }
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


void CreateModulWindow::on_cbModul_editTextChanged(const QString &arg1)
{
    proxy.setFilterFixedString(arg1);
}

