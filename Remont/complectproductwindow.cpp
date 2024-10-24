#include "complectproductwindow.h"
#include "scan.h"
#include "selectdevicewindow.h"
#include "ui_complectproductwindow.h"

#include <models/modul.h>

ComplectProductWindow::ComplectProductWindow(QWidget *parent, Product *product)
    : QDialog(parent)
    , ui(new Ui::ComplectProductWindow)
{
    ui->setupUi(this);

    if(product != nullptr)
    {
        prod = *product;
        ui->leNumProdSearch->setVisible(false);
        ui->tbProdSearch->setVisible(false);
        ui->labelSearchProd->setVisible(false);
        LoadProductToScreen(prod);
    }

    connect(ui->lwOuterModule, SIGNAL(doubleClicked(QModelIndex)), SLOT(on_pbAddModul_clicked()));
    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));

}

ComplectProductWindow::~ComplectProductWindow()
{
    disconnect(conn);
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
    QVector<Status::Stat> stat {Status::CREATE};
    SelectDeviceWindow *win = new SelectDeviceWindow(this);
    win->setTypeSearch(SelectDeviceWindow::TypeProduct);
    IDevice *dev = win->SelectDevice(true, ui->leNumProdSearch->text(), stat);
    if(dev != nullptr)
    {
        prod = *(static_cast<Product*>(dev));
        if(prod.id > 0)
            LoadProductToScreen(prod);
    }
}


//----------------------------------------------------------------------------------------------
// Вывод данных продукта на экран
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::LoadProductToScreen(Product &prod)
{
    repo.LoadChildProduct(prod);
    ui->lbNameProd->setText(prod.name);
    ui->lbNumProd->setText(prod.number);

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
    }
}

//----------------------------------------------------------------------------------------------
// Добавление модуля в экранной форме и удаление из списка модулей
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::addModulToScreen(Modul &mod)
{
    if(trackModul.AddRecord(mod.id, mod))
    {
        // добавление в список экрана изделия
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(mod.number + " (" + mod.name + ")");
        QVariant var;
        var.setValue(mod);
        item->setData(Qt::UserRole, var);
        ui->lwInnerModule->addItem(item);

        // удаление из списка модулей
        for(int row = 0; row < ui->lwOuterModule->count(); ++row)
        {
            auto item = ui->lwOuterModule->item(row);
            QVariant var = item->data(Qt::UserRole);
            Modul mod2 = var.value<Modul>();
            if(mod2.id == mod.id)
            {
                delete item;
                break;
            }
        }
    }
}


//----------------------------------------------------------------------------------------------
// Добавление модуля в изделие
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_pbAddModul_clicked()
{
    if(prod.id == 0 || ui->lwOuterModule->currentRow() < 0)
        return;

    QVariant var = ui->lwOuterModule->item(ui->lwOuterModule->currentRow())->data(Qt::UserRole);
    Modul mod = var.value<Modul>();

    addModulToScreen(mod);

    // trackModul.AddRecord(mod.id, mod);

    // QListWidgetItem *item = new QListWidgetItem;
    // item->setText(mod.number + " (" + mod.name + ")");
    // var.setValue(mod);
    // item->setData(Qt::UserRole, var);
    // ui->lwInnerModule->addItem(item);

    // удаление из списка модулей
    // delete ui->lwOuterModule->item(ui->lwOuterModule->currentRow());
}


//----------------------------------------------------------------------------------------------
// Удаление модуля из изделие
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_pbDeleteModul_clicked()
{
    if(ui->lwInnerModule->currentRow() < 0)
        return;

    QVariant var = ui->lwInnerModule->item(ui->lwInnerModule->currentRow())->data(Qt::UserRole);
    Modul mod = var.value<Modul>();

    trackModul.DelRecord(mod.id, mod);
    // if(addModul.contains(mod))
    //     addModul.remove(mod);
    // else
    //     delModul.insert(mod);

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
    // QSet<Modul> res = addModul;
    // res.intersect(delModul);
    // addModul.subtract(res);
    // delModul.subtract(res);

    // trackModul.setResult();

    QList<Modul> addModul;
    trackModul.getListAdd(addModul);
    // Добавление модулей в изделие и изменение статуса на  Установлен в оборудование

    for(auto &it : addModul)
    {
        Modul mod = it;
        // Status status;
        // status.dateStatus = QDateTime::currentDateTime();
        // status.idStatus = Status::INSTALL;
        // status.idDevice = mod.id;
        // mod.listStatus.push_back(status);
        mod.idProduct = prod.id;
        // записать в базу новый статус и id изделия для модуля
        if(repo.UpdateItem(mod))
            mod.AddStatus(mod, Status::INSTALL);

    }

    // Удаление модулей из изделия и изменение статуса на исправен на производстве
    QList<Modul> delModul;
    trackModul.getListDel(delModul);
    for(auto &it : delModul)
    {
        Modul mod = it;
        mod.idProduct = 0;
        if(repo.UpdateItem(mod))
            mod.DeleteLastStatus(mod);

        mod.listStatus.removeIf( [] (auto n) { return n.idStatus == Status::INSTALL; });
    }

    accept();
}



void ComplectProductWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        ui->leNumProdSearch->setText(s);
        Product prod2 = repo.GetProduct(s, Status::CREATE);
        if(prod2.id > 0)
        {
            prod = prod2;
            LoadProductToScreen(prod);
            return;
        }

        if(prod.id > 0)
        {
            Modul mod = repo.GetModul(s, Status::CREATE);
            if(mod.id > 0)
                addModulToScreen(mod);
        }
    }
}



