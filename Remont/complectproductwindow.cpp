#include "complectproductwindow.h"
#include "scan.h"
#include "selectdevicewindow.h"
#include "ui_complectproductwindow.h"
#include <QPointer>


ComplectProductWindow::ComplectProductWindow(QWidget *parent, Items *_item)
    : QDialog(parent)
    , ui(new Ui::ComplectProductWindow)
{
    ui->setupUi(this);

    if(_item != nullptr)
    {
        dev = *_item;
        repo.LoadChildItems(dev.id, dev.childItems);
        ui->leNumProdSearch->setVisible(false);
        ui->tbProdSearch->setVisible(false);
        ui->labelSearchProd->setVisible(false);
        ui->tbClear->setVisible(false);
        LoadProductToScreen(dev);
    }

    conn = connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));
    connect(ui->wTree, &TreeItemsForm::currentItemChanged, this, &ComplectProductWindow::UpdateButtonEnabled);

    UpdateButtonEnabled();
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

    QVector<StatusItem> stat {StatusItem::CREATE, StatusItem::CORRECT, StatusItem::CORRECT_OSO};
    QPointer<SelectDeviceWindow> win;

    if(dev.type.indexType == IndexType::Product)
    {
        win = new SelectDeviceWindow(IndexType::Modul, this);
        win->AddSelectedType(IndexType::Plate);
    }
    else
    {
        win = new SelectDeviceWindow(IndexType::Plate, this);
    }

    win->ExcludeDevice(listAddId);

    Items *child = win->SelectDevice(true, stat, ui->leNumModSearch->text(), false, LoadPartType::NO_HAS_PARENT);
    if(child != nullptr && win->result() == QDialog::Accepted)
    {
        addModulToScreen(*child);
        listAddId.insert(child->id);
    }

}


//----------------------------------------------------------------------------------------------
// Кнопка поиска изделия
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_tbProdSearch_clicked()
{
    QVector<StatusItem> stat {StatusItem::CREATE, StatusItem::CORRECT, StatusItem::CORRECT_OSO, StatusItem::FAULTY};

    QScopedPointer<SelectDeviceWindow> win (new SelectDeviceWindow(IndexType::Product, this));
    win->AddSelectedType(IndexType::Modul);
    Items *res = win->SelectDevice(true, stat, ui->leNumProdSearch->text(), false, LoadPartType::NO_HAS_PARENT);
    if(res != nullptr && win->result() == QDialog::Accepted)
    {
        ui->wTree->Clear();
        dev = *res;
        repo.LoadChildItems(dev.id, dev.childItems);
        LoadProductToScreen(dev);
        UpdateButtonEnabled();
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
    ui->wTree->AddItem(&dev, false);

}

//----------------------------------------------------------------------------------------------
// Добавление модуля в экранной форме и удаление из списка модулей
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::addModulToScreen(Items &mod)
{
    if(mod.id > 0 && trackModul.AddRecord(/*mod.id,*/ mod))
    {
        ui->wTree->AddItem(&mod);
        UpdateButtonEnabled();
    }
}

//---------------------------------------------------------------------------
// Включение кнопок
//---------------------------------------------------------------------------
void ComplectProductWindow::UpdateButtonEnabled()
{
    ui->pbDeleteModul->setEnabled(ui->wTree->GetSelectedId() > 0);
    ui->tbSearchModul->setEnabled(dev.id > 0);
}


//----------------------------------------------------------------------------------------------
// Удаление модуля из изделие
//----------------------------------------------------------------------------------------------
void ComplectProductWindow::on_pbDeleteModul_clicked()
{
    int id = ui->wTree->DeleteSelectedItem(true);

    if(id == 0)
        return;

    Items mod = repo.GetItem(id);
    Q_ASSERT(mod.id != 0);
    trackModul.DelRecord(/*mod.id,*/ mod);
    listAddId.remove(mod.id);
    UpdateButtonEnabled();

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
            mod.AddStatus(mod, StatusItem::INSTALL);

    }

    // Удаление модулей из изделия и изменение статуса на исправен на производстве
    QList<Items> delModul;
    trackModul.getListDel(delModul);
    for(auto &it : delModul)
    {
        Items mod = it;
        mod.idParent = 0;
        if(repo.UpdateItem(mod))
            mod.DeleteLastStatus(mod, StatusItem::INSTALL);

        mod.listStatus.removeIf( [] (auto n) { return n.idStatus == StatusItem::INSTALL; });
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
        QVector<StatusItem> stat = {StatusItem::CREATE, StatusItem::FAULTY};
        Items item = repo.GetItem2(s, stat);

        if(item.id > 0)
        {
            // добавление в изделия
            if((dev.id == 0 && item.type.indexType != IndexType::Plate)
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

            UpdateButtonEnabled();
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
    UpdateButtonEnabled();
}

