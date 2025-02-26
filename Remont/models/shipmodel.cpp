#include "shipmodel.h"
#include "listshipment.h"

ShipModel::ShipModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    createList();
    listShip->GetHeader(headers);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
QVariant ShipModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant var;
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            var = headers[section];
        }
        else
        {
            var = section;
        }
    }
    return var;
}


//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
int ShipModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return listShip->items.size();
    // return 0;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
int ShipModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.size();
}

// bool ShipModel::hasChildren(const QModelIndex &parent) const
// {
//     // FIXME: Implement me!
// }

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool ShipModel::canFetchMore(const QModelIndex &/*parent*/) const
{
    return isFetch;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ShipModel::fetchMore(const QModelIndex &/*parent*/)
{
    if(!isBaseOff)
    {
        int resLoad = listShip->LoadPart(startLoad, cntLoad, number, isShip);

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


//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
QVariant ShipModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (!index.isValid())
        return var;

    var = listShip->getData(index.row(), index.column(), role);

    return var;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool ShipModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        listShip->setData(index.row(), index.column(), value, role);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}


//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
// Qt::ItemFlags ShipModel::flags(const QModelIndex &index) const
// {
//     if (!index.isValid())
//         return Qt::NoItemFlags;

//     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
// }

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool ShipModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}


//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool ShipModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    listShip->DeleteItem(row);
    endRemoveRows();
    return true;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ShipModel::prepareLoad(const QString _number, bool _isShip)
{
    number = _number;
    isShip = _isShip;

    listShip->items.clear();

    startLoad = 0;
    cntLoad = 60;
    isFetch = true;
    beginResetModel();
    endResetModel();

}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
Shipment *ShipModel::GetItem(int row)
{
    return listShip->GetItemAtRow(row);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool ShipModel::DeleteItem(int row)
{
    return removeRow(row);
}


void ShipModel::DeleteItemFromList(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    listShip->DeleteItemFromList(row);
    endRemoveRows();
}


//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ShipModel::createList()
{
    listShip = std::make_unique<ListShipment>();
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ShipModel::AddItem(Shipment *item)
{
    int row = listShip->items.size();
    if(listShip->AddItem(*item))
        insertRows(row, 1);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ShipModel::AddItemToList(Shipment *item)
{
    int row = listShip->items.size();
    listShip->AddItemToList(item);
    insertRows(row, 1);

}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ShipModel::UpdateItem(int row)
{
    Shipment* item = listShip->GetItemAtRow(row);
    if(item != nullptr)
        listShip->UpdateItem(*item);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
// void ShipModel::UpdateItemModel(int row)
// {

// }

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ShipModel::setBaseOff()
{
    isBaseOff = true;
}

