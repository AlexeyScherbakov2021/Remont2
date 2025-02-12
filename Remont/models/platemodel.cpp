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

PlateModel::~PlateModel()
{

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

bool PlateModel::hasChildren(const QModelIndex &/*parent*/) const
{
    return false;
}

bool PlateModel::canFetchMore(const QModelIndex &/*parent*/) const
{
    return isFetch;
}

void PlateModel::fetchMore(const QModelIndex &/*parent*/)
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

QVariant PlateModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (!index.isValid())
        return var;

    var = listDev->getData(index.row(), index.column(), role);

    return var;
}

bool PlateModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

void PlateModel::prepareLoad2(const QString _number, QVector<int>& _status, bool _isBusy, bool _isParent)
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


Items *PlateModel::GetItem(int row)
{
    if(row >= 0 && row < listDev->items.size())
        return &listDev->items[row];
    else
        return nullptr;
}

bool PlateModel::DeleteItem(int row)
{
    removeRows(row, 1);
    return true;
}

void PlateModel::DeleteItemFromList(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    listDev->DeleteItemFromList(row);
    endRemoveRows();
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
    case ItemType::All:
        listDev = std::make_unique<ListDevice>(ItemType::All);
        break;
    default:
        Q_ASSERT(false);
    }
}

// void PlateModel::setFunction(pLoadItems p)
// {
//     lp = p;
// }

void PlateModel::AddItem(Items *item)
{
    int row = listDev->items.size();
    listDev->items.push_back(*item);
    insertRows(row, 1);
}

void PlateModel::UpdateItem(int row)
{
    Items* item = GetItem(row);
    if(item->id > 0)
        listDev->UpdateItem(*item);
}

void PlateModel::setBaseOff()
{
    isBaseOff = true;
}





