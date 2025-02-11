#include "complectproductwindow.h"
#include "scan.h"
#include "selectdevicewindow.h"
#include "ui_complectproductwindow.h"

#include <QPointer>

// #include <models/modul.h>

ComplectProductWindow::ComplectProductWindow(QWidget *parent, Items *_item)
    : QDialog(parent)
    , ui(new Ui::ComplectProductWindow)
{
    ui->setupUi(this);

    if(_item != nullptr)
    {
        dev = *_item;
        ui->leNumProdSearch->setVisible(false);
        ui->tbProdSearch->setVisible(false);
        ui->labelSearchProd->setVisible(false);
        ui->tbClear->setVisible(false);
        LoadProductToScreen(dev);

    }

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

    if(dev.id == 0)
        return;

    QVector<int> stat {Status::CREATE, Status::CORRECT, Status::CORRECT_OSO};
    QPointer<SelectDeviceWindow> win;

    if(dev.type.indexType == ItemType::Product)
    {
        win = new SelectDeviceWindow(ItemType::Modul, this);
        win->AddSelectedType(ItemType::Plate);
    }
    else
    {
        win = new SelectDeviceWindow(ItemType::Plate, this);
    }

    Items child = *win->SelectDevice(true, stat, ui->leNumModSearch->text(), false, false);
    if(win->result() == QDialog::Accepted)
    {
        addModulToScreen(child);
    }
}


//----------------------------------------------------------------------------------------------
// Кнопка поиска изделия
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_tbProdSearch_clicked()
{
    QVector<int> stat {Status::CREATE, Status::CORRECT, Status::CORRECT_OSO};
    QPointer<SelectDeviceWindow> win = new SelectDeviceWindow(ItemType::Product, this);
    win->AddSelectedType(ItemType::Modul);
    dev = *win->SelectDevice(true, stat, ui->leNumProdSearch->text(), false, false);
    if(win->result() == QDialog::Accepted)
    {
        repo.LoadChildItems(dev.id, dev.childItems);
        LoadProductToScreen(dev);
    }

}


//----------------------------------------------------------------------------------------------
// Вывод данных продукта на экран
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::LoadProductToScreen(Items &dev)
{
    trackModul.clear();

    ui->lbNameProd->setText(dev.name);
    ui->lbNumProd->setText(dev.number);
    ui->lbVNFT->setText(dev.VNFT);

    QString nameType;
    QString nameIcon;
    dev.GetInfo(nameType, nameIcon);
    ui->imageDev->setPixmap(QPixmap(nameIcon));
    ui->imageDev->setToolTip(nameType);

    ui->lwInnerModule->clear();
    for(auto &it : dev.childItems)
    {
        ShowLineChild(it);
    }
}

//----------------------------------------------------------------------------------------------
// Добавление модуля в экранной форме и удаление из списка модулей
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::addModulToScreen(Items &mod)
{
    if(trackModul.AddRecord(mod.id, mod))
    {
        ShowLineChild(mod);
    }
}

//----------------------------------------------------------------------------------------------
// Отображение строки вложенного устройства
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::ShowLineChild(Items& child)
{
    QString nameType;
    QString iconName;
    QListWidgetItem *item = new QListWidgetItem;

    child.GetInfo(nameType, iconName);
    item->setText(child.number + " (" + child.type.typeName + " " + child.VNFT + ")");
    item->setIcon(QIcon(iconName));
    QVariant var;
    var.setValue(child);
    item->setData(Qt::UserRole, var);
    ui->lwInnerModule->addItem(item);

}

//----------------------------------------------------------------------------------------------
// Удаление модуля из изделие
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_pbDeleteModul_clicked()
{
    if(ui->lwInnerModule->currentRow() < 0)
        return;

    QVariant var = ui->lwInnerModule->item(ui->lwInnerModule->currentRow())->data(Qt::UserRole);
    Items mod = var.value<Items>();

    trackModul.DelRecord(mod.id, mod);

    QListWidgetItem *item = new QListWidgetItem;
    item->setText(mod.number + " (" + mod.name + ")");
    var.setValue(mod);
    item->setData(Qt::UserRole, var);

    delete ui->lwInnerModule->item(ui->lwInnerModule->currentRow());

}


//----------------------------------------------------------------------------------------------
// Кнопка подтверждения изменений
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_pbOK_clicked()
{
    QList<Items> addModul;
    trackModul.getListAdd(addModul);
    // Добавление модулей в изделие и изменение статуса на  Установлен в оборудование

    for(auto &it : addModul)
    {
        Items mod = it;
        mod.idParent = dev.id;
        // записать в базу новый статус и id изделия для модуля
        if(repo.UpdateItem(mod))
            mod.AddStatus(mod, Status::INSTALL);

    }

    // Удаление модулей из изделия и изменение статуса на исправен на производстве
    QList<Items> delModul;
    trackModul.getListDel(delModul);
    for(auto &it : delModul)
    {
        Items mod = it;
        mod.idParent = 0;
        if(repo.UpdateItem(mod))
            mod.DeleteLastStatus(mod);

        mod.listStatus.removeIf( [] (auto n) { return n.idStatus == Status::INSTALL; });
    }

    accept();
}


//----------------------------------------------------------------------------------------------
// Событие чтения сканера
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        QVector<int> stat = {Status::CREATE};
        Items item = repo.GetItem2(s, stat);

        if(item.id > 0)
        {
            // добавление в изделия
            if(dev.id == 0 && item.type.indexType != ItemType::Plate
                    || dev.type.indexType == item.type.indexType)
            {
                ui->leNumProdSearch->setText(s);
                dev = item;
                repo.LoadChildItems(dev.id, dev.childItems);
                LoadProductToScreen(dev);
            }
            // добавление во вложения
            else if(dev.type.indexType < item.type.indexType)
            {
                ui->leNumModSearch->setText(s);
                addModulToScreen(item);
            }
        }
    }
}


//----------------------------------------------------------------------------------------------
// Кнопка Очистить
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_tbClear_clicked()
{
    Items item;
    LoadProductToScreen(item);
}

