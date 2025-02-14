#include "devicemodel.h"

#include "listclaim.h"
#include "listmodul.h"
#include "listproduct.h"

#include <QIcon>

DeviceModel::DeviceModel(IndexType type, QObject *parent)
    : QAbstractTableModel(parent)
{
    createList(type);
    listDev->GetHeader(headers);
}

DeviceModel::~DeviceModel()
{

}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant var;
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
            var = headers[section];
        else
            var = section + 1;
    }

    return var;
}


int DeviceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return listDev->items.size();
}

int DeviceModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.size();
}

bool DeviceModel::hasChildren(const QModelIndex &/*parent*/) const
{
    return false;
}

bool DeviceModel::canFetchMore(const QModelIndex &/*parent*/) const
{
    return isFetch;
}

void DeviceModel::fetchMore(const QModelIndex &/*parent*/)
{

    if(!isBaseOff)
    {
        int resLoad = listDev->LoadPart2(startLoad, cntLoad, number, vStatus, isBusy, isParent);

        if(resLoad > 0)
        {
            beginInsertRows(QModelIndex(), startLoad, startLoad + resLoad - 1);
            endInsertRows();
        }

        startLoad += resLoad;
        if(resLoad < cntLoad || resLoad == 0)
            isFetch = false;
    }
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (!index.isValid())
        return var;

    var = listDev->getData(index.row(), index.column(), role);

    return var;
}

bool DeviceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {

        listDev->setData(index.row(), index.column(), value, role);

        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

// Qt::ItemFlags PlateModel::flags(const QModelIndex &index) const
// {
//     if (!index.isValid())
//         return Qt::NoItemFlags;

//     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
// }

bool DeviceModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool DeviceModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(count <= 0)
        return false;
    beginRemoveRows(parent, row, row + count - 1);

    listDev->DeleteItem(row);

    endRemoveRows();
    return true;
}

void DeviceModel::prepareLoad(const QString _number, int _status, bool _isBusy, bool _isParent)
{
    Q_ASSERT_X(listDev != nullptr, "", "not calling createList()");

    beginResetModel();
    startLoad = 0;
    listDev->items.clear();
    status = _status;
    isBusy = _isBusy;
    isParent = _isParent;
    number = _number;
    isFetch = true;
    endResetModel();

}

void DeviceModel::prepareLoad2(const QString _number, QVector<int>& _status, bool _isBusy, bool _isParent)
{
    // qDebug() << "prepareLoad2" ;

    Q_ASSERT_X(listDev != nullptr, "", "not calling createList()");

    beginResetModel();
    startLoad = 0;
    listDev->items.clear();
    vStatus = _status;
    isBusy = _isBusy;
    isParent = _isParent;
    number = _number;
    isFetch = true;
    endResetModel();

}


Items *DeviceModel::GetItem(int row)
{
    if(row >= 0 && row < listDev->items.size())
        return &listDev->items[row];
    else
        return nullptr;
}

bool DeviceModel::DeleteItem(int row)
{
    removeRows(row, 1);
    return true;
}

void DeviceModel::DeleteItemFromListId(int id)
{
    int row = listDev->GetRowFromId(id);
    if(row >= 0)
        DeleteItemFromList(row);
}

void DeviceModel::DeleteItemFromList(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    listDev->DeleteItemFromList(row);
    endRemoveRows();
}


void DeviceModel::createList(IndexType type)
{
    switch(type)
    {
    case IndexType::Product:
        listDev = std::make_unique<ListProduct>();
        break;
    case IndexType::Modul:
        listDev = std::make_unique<ListModul>();
        break;
    case IndexType::Plate:
        listDev = std::make_unique<ListPlate>();
        break;
    case IndexType::All:
        listDev = std::make_unique<ListDevice>(IndexType::All);
        break;

    case IndexType::ClaimType:
        // listDev = std::make_unique<ListClaim>();
        break;
    default:
        Q_ASSERT(false);
    }
}

// void DeviceModel::setFunction(pLoadItems p)
// {
//     lp = p;
// }

void DeviceModel::AddItem(Items *item)
{
    int row = listDev->items.size();
    listDev->items.push_back(*item);
    insertRows(row, 1);
}

void DeviceModel::UpdateItem(int row)
{
    Items* item = GetItem(row);
    if(item->id > 0)
        listDev->UpdateItem(*item);
}

void DeviceModel::setBaseOff()
{
    isBaseOff = true;
}

// Items* DeviceModel::GetItem(QString number)
// {
//     return listDev->GetItem(number);
// }





