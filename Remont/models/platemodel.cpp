#include "platemodel.h"

#include "listmodul.h"
#include "listproduct.h"

#include <QIcon>

PlateModel::PlateModel(ItemType::IndexType type, QObject *parent)
    : QAbstractTableModel(parent)
{
    createList(type);
    listDev->GetHeader(headers);
}

QVariant PlateModel::headerData(int section, Qt::Orientation orientation, int role) const
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


int PlateModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return listDev->items.size();
}

int PlateModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.size();
}

bool PlateModel::hasChildren(const QModelIndex &parent) const
{
    return false;
}

bool PlateModel::canFetchMore(const QModelIndex &parent) const
{
    return isFetch;
}

void PlateModel::fetchMore(const QModelIndex &parent)
{
    int resLoad = listDev->LoadPart(startLoad, cntLoad, number, status, isBusy, isParent);

    beginInsertRows(QModelIndex(), startLoad, startLoad + resLoad - 1);
    endInsertRows();

    startLoad += resLoad;
    if(resLoad < cntLoad)
        isFetch = false;

}

QVariant PlateModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (!index.isValid())
        return var;

    var = listDev->getData(index.row(), index.column(), role);

    return var;
}

// bool PlateModel::setData(const QModelIndex &index, const QVariant &value, int role)
// {
//     if (data(index, role) != value) {
//         // FIXME: Implement me!
//         emit dataChanged(index, index, {role});
//         return true;
//     }
//     return false;
// }

// Qt::ItemFlags PlateModel::flags(const QModelIndex &index) const
// {
//     if (!index.isValid())
//         return Qt::NoItemFlags;

//     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
// }

bool PlateModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool PlateModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(count <= 0)
        return false;
    beginRemoveRows(parent, row, row + count - 1);

    listDev->DeleteItem(row);

    endRemoveRows();
    return true;
}

void PlateModel::prepareLoad(const QString _number, int _status, bool _isBusy, bool _isParent)
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

Items *PlateModel::GetItem(int index)
{
    if(index >= 0 && index < listDev->items.size())
        return &listDev->items[index];
    else
        return nullptr;
}

bool PlateModel::DeleteItem(int row)
{
    removeRows(row, 1);
    return true;
}


void PlateModel::createList(ItemType::IndexType type)
{
    switch(type)
    {
    case ItemType::Product:
        listDev = std::make_unique<ListProduct>();
        break;
    case ItemType::Modul:
        listDev = std::make_unique<ListModul>();
        break;
    case ItemType::Plate:
        listDev = std::make_unique<ListPlate>();
        break;
    default:
        Q_ASSERT(false);
    }
}

// bool PlateModel::removeColumns(int column, int count, const QModelIndex &parent)
// {
//     beginRemoveColumns(parent, column, column + count - 1);
//     // FIXME: Implement me!
//     endRemoveColumns();
//     return true;
// }

