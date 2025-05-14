#include "cardprodwindow.h"
#include "scan.h"
#include "selectdevicewindow.h"
#include "ui_selectdevicewindow.h"

#include <QClipboard>



SelectDeviceWindow::SelectDeviceWindow(IndexType _type, QWidget *parent)
    : QDialog(parent), ui(new Ui::SelectDeviceWindow), type(_type)
{
    ui->setupUi(this);

    proxy = new ModelProxy(this);
    model = new DeviceModel(type, this);
    proxy->setSourceModel(model);

    AddSelectedType(type);
    ui->cbType->setModel(&typeModel);

    ui->tableView->setModel(proxy);
    ui->tableView->setColumnWidth(0, 30);       // icon
    ui->tableView->setColumnWidth(1, 100);      // Номер
    ui->tableView->setColumnWidth(2, 200);      // тип
    ui->tableView->setColumnWidth(3, 200);      // внфт
    ui->tableView->setColumnWidth(4, 150);      // наименование
    ui->tableView->setColumnWidth(5, 80);       // дата регистрации
    ui->tableView->setColumnWidth(6, 80);       // статус

    connect(&Scan::scan, SIGNAL(sigRead(QString)), SLOT(slotReadScan(QString)));


    ui->tableView->addAction("Карточка устройства", this, [this] () {

        QModelIndex index = ui->tableView->currentIndex();
        index = proxy->mapToSource(index);
        Items* dev = model->GetItem(&index);
        if(dev->id > 0 && dev->type.indexType <= IndexType::Plate)
        {
            CardProdWindow *win = new CardProdWindow(dev, this);
            win->exec();
        }

    });
    ui->tableView->addAction("Скопировать номер", this, [this] () {
        QModelIndex index = ui->tableView->currentIndex();
        index = proxy->mapToSource(index);
        Items* dev = model->GetItem(&index);
        QClipboard *cpb = QApplication::clipboard();
        cpb->setText(dev->number, QClipboard::Clipboard);
    });

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

}


SelectDeviceWindow::~SelectDeviceWindow()
{
    delete ui;
    // delete device;

    // qDebug() << "destructor SelectDeviceWindow";
}

//---------------------------------------------------------------------------------------
// Добавление нового типа для поиска
//---------------------------------------------------------------------------------------
void SelectDeviceWindow::AddSelectedType(IndexType _type)
{
    int row = typeModel.rowCount();
    typeModel.insertRow(row);

    QString name;
    QString sIcon;

    Items dev;
    dev.type.indexType = _type;
    dev.GetInfo(name, sIcon);

    QStandardItem *item = new QStandardItem(QIcon(sIcon), name);
    item->setData(_type, Qt::UserRole);
    typeModel.setItem(row, 0, item);
}

//---------------------------------------------------------------------------------------
// Исключить из списка набор ID
//---------------------------------------------------------------------------------------
void SelectDeviceWindow::ExcludeDevice(QSet<int>& _setId)
{
    setId = _setId;
    proxy->AddExcludeListId(setId);
}

//---------------------------------------------------------------------------------------
// Начало нового запроса
//---------------------------------------------------------------------------------------
void SelectDeviceWindow::startLoad()
{
    model->prepareLoad2(ui->leSearch->text(), vStatus, isBusy, hasParent );
}


//--------------------------------------------------------------------------------------------------
// Поиск устройства по списку статусов
//--------------------------------------------------------------------------------------------------
Items *SelectDeviceWindow::SelectDevice(bool isNow, const QVector<StatusItem> &statusList, QString searchNum, bool _isBusy, LoadPartType _hasParent)
{
    vStatus = statusList;
    isBusy = _isBusy;
    hasParent = _hasParent;
    vStatus = statusList;

    ui->leSearch->setText(searchNum);

    QList<Items> listTemp;
    repo.LoadPart(0, 4, type, searchNum, listTemp, statusList, isBusy, hasParent);

    if(isNow && !searchNum.isEmpty())
    {
        if(listTemp.size() == 1 && searchNum == listTemp.first().number)
        {
            device = listTemp.first();
            accept();
            return &device;
        }
    }

    connect(ui->cbType, SIGNAL(currentIndexChanged(int)), SLOT(slotTypeChanged(int)));
    startLoad();
    exec();

    if(device.id == 0)
        return nullptr;
    else
        return &device;
}


//--------------------------------------------------------------------------------------------------
// Установка флага немедленного поиска
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::setDisableSearch()
{
    ui->leSearch->setEnabled(false);
    ui->pbSelect->setEnabled(false);
}


//--------------------------------------------------------------------------------------------------
// Искать по номеру и установленным параметрам
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::Search(QString /*number*/)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    startLoad();
    QApplication::restoreOverrideCursor();
}


//--------------------------------------------------------------------------------------------------
// Кнопка Искать
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::on_tbSearch_clicked()
{
    Search(ui->leSearch->text());
}


//--------------------------------------------------------------------------------------------------
// Кнопка Выбрать
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::on_pbSelect_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index == QModelIndex() || ui->leSearch->hasFocus())
    {
        on_tbSearch_clicked();
        return;
    }

    index = proxy->mapToSource(index);

    device = *model->GetItem(&index);
    accept();
}



//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::on_tableView_doubleClicked(const QModelIndex &/*index*/)
{
    on_pbSelect_clicked();
}



//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::slotTypeChanged(int row)
{
    QModelIndex index = typeModel.index(row, 0);

    IndexType type = (IndexType)typeModel.data(index, Qt::UserRole).toInt();
    delete model;
    model = new DeviceModel(type, this);
    proxy->setSourceModel(model);
    proxy->AddExcludeListId(setId);
    startLoad();
    ui->tableView->setModel(proxy);

}

//--------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------
void SelectDeviceWindow::slotReadScan(QString s)
{
    if(isActiveWindow())
    {
        ui->leSearch->setText(s);
        on_tbSearch_clicked();
    }
}

