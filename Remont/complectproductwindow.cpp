#include "complectproductwindow.h"
#include "selectdevicewindow.h"
#include "ui_complectproductwindow.h"

#include <models/modul.h>

ComplectProductWindow::ComplectProductWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ComplectProductWindow)
{
    ui->setupUi(this);

}

ComplectProductWindow::~ComplectProductWindow()
{
    delete ui;
}


//----------------------------------------------------------------------------------------------
// Кнопка поиска модулей
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_tbSearchModul_clicked()
{
    // поиск модулей со статусом Исправен на производстве для изделия со статусом Создан

    ui->lwOuterModule->clear();
    Modules.listItems.clear();
    QApplication::setOverrideCursor(Qt::WaitCursor);

    int status = ui->rbNewModule->isChecked() ? Status::CREATE : Status::FAULTY;

    Modules.FindItems(ui->leNumModSearch->text(), status);
    // repo.FindModulsStatus(ui->leNumModSearch->text(), listModul, status);

    for(const auto &it : Modules.listItems)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(it.number + " (" + it.name + ")");
        QVariant var;
        var.setValue(it);
        item->setData(Qt::UserRole, var);
        ui->lwOuterModule->addItem(item);
    }

    QApplication::restoreOverrideCursor();
}


//----------------------------------------------------------------------------------------------
// Кнопка поиска изделия
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_tbProdSearch_clicked()
{
    SelectDeviceWindow *win = new SelectDeviceWindow(this, ui->leNumProdSearch->text(), Status::CREATE, SelectDeviceWindow::TypeProduct);
    if(win->exec() != QDialog::Accepted)
    {
        return;
    }

    // prod = repo.GetProduct(ui->leNumProdSearch->text());
    prod = win->prod;
    if(prod.id == 0)
        return;

    repo.LoadChildProduct(prod);
    ui->lbNameProd->setText(prod.name);
    ui->lbNumProd->setText(prod.number);
    // ui->lbTypeProd->setText(prod.);

    ui->lwInnerModule->clear();
    for(auto &it : prod.listModules)
    {
        repo.LoadStatus(it);
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(it.number + " (" + it.name + ")");
        QVariant var;
        var.setValue(it);
        item->setData(Qt::UserRole, var);
        ui->lwInnerModule->addItem(item);

        // ui->lwInnerModule->addItem(it.number + " (" + it.name + ")");
    }
}

//----------------------------------------------------------------------------------------------
// Добавление модуля в изделие
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_pbAddModul_clicked()
{
    if(prod.id == 0)
        return;

    QVariant var = ui->lwOuterModule->item(ui->lwOuterModule->currentRow())->data(Qt::UserRole);
    Modul mod = var.value<Modul>();

    // добавление в список отслеживания
    addModul.insert(mod);

    // Добавление в список изделия

    // добавление в список экрана изделия
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(mod.number + " (" + mod.name + ")");
    var.setValue(mod);
    item->setData(Qt::UserRole, var);
    ui->lwInnerModule->addItem(item);


    // удаление из списка модулей
    delete ui->lwOuterModule->item(ui->lwOuterModule->currentRow());

}


//----------------------------------------------------------------------------------------------
// Удаление модуля из изделие
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_pbDeleteModul_clicked()
{
    QVariant var = ui->lwInnerModule->item(ui->lwInnerModule->currentRow())->data(Qt::UserRole);
    Modul mod = var.value<Modul>();

    delModul.insert(mod);

    QListWidgetItem *item = new QListWidgetItem;
    item->setText(mod.number + " (" + mod.name + ")");
    var.setValue(mod);
    item->setData(Qt::UserRole, var);
    ui->lwOuterModule->addItem(item);

    delete ui->lwInnerModule->item(ui->lwInnerModule->currentRow());

}


//----------------------------------------------------------------------------------------------
// Кнопка подтверждения изменений
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_pbOK_clicked()
{
    QSet<Modul> res = addModul;
    res.intersect(delModul);
    addModul.subtract(res);
    delModul.subtract(res);

    // Добавление модулей в изделие и изменение статуса на  Установлен в оборудование

    for(auto &it : addModul)
    {
        Modul mod = it;
        Status status;
        status.dateStatus = QDateTime::currentDateTime();
        status.idStatus = Status::INSTALL;
        status.idDevice = mod.id;
        mod.listStatus.push_back(status);
        mod.idProduct = prod.id;
        // записать в базу новый статус и id изделия для модуля
        if(repo.UpdateItem(mod))
            mod.AddStatus(mod, Status::INSTALL);
            // repo.AddStatusModul(status);

    }

    // Удаление модулей из изделия и изменение статуса на исправен на производстве
    for(auto &it : delModul)
    {
        Modul mod = it;
        mod.idProduct = 0;
        repo.UpdateItem(mod);
        // repo.DeleteStatusModul();

        mod.listStatus.removeIf( [] (auto n) { return n.idStatus == Status::INSTALL; });
    }

    accept();
}

