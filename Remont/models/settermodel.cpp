#include "settermodel.h"

SetterModel::SetterModel( QObject *parent) : QAbstractTableModel(parent)
{
    createList();
    listSetter->GetHeader(headers);
}

QVariant SetterModel::headerData(int section, Qt::Orientation orientation, int role) const
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

// bool SetterModel::setHeaderData(int section,
//                                 Qt::Orientation orientation,
//                                 const QVariant &value,
//                                 int role)
// {
//     if (value != headerData(section, orientation, role)) {
//         // FIXME: Implement me!
//         emit headerDataChanged(orientation, section, section);
//         return true;
//     }
//     return false;
// }

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
int SetterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return listSetter->items.size();
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
int SetterModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.size();
}

// bool SetterModel::hasChildren(const QModelIndex &parent) const
// {
//     // FIXME: Implement me!
// }

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool SetterModel::canFetchMore(const QModelIndex &/*parent*/) const
{
    return isFetch;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void SetterModel::fetchMore(const QModelIndex &/*parent*/)
{
    if(!isBaseOff)
    {
        int resLoad = listSetter->LoadPart(startLoad, cntLoad, number, isFree);

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
QVariant SetterModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (!index.isValid())
        return var;

    var = listSetter->getData(index.row(), index.column(), role);

    return var;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool SetterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        listSetter->setData(index.row(), index.column(), value, role);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

void SetterModel::setData(const QModelIndex &index, SetterOut *item)
{
    listSetter->setItem(index.row(), item);
}

// Qt::ItemFlags SetterModel::flags(const QModelIndex &index) const
// {
//     if (!index.isValid())
//         return Qt::NoItemFlags;

//     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
// }

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool SetterModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

// bool SetterModel::insertColumns(int column, int count, const QModelIndex &parent)
// {
//     beginInsertColumns(parent, column, column + count - 1);
//     // FIXME: Implement me!
//     endInsertColumns();
//     return true;
// }

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool SetterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    listSetter->DeleteItem(row);
    endRemoveRows();
    return true;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void SetterModel::prepareLoad(const QString _number, bool _isFree)
{
    number = _number;
    isFree = _isFree;

    listSetter->items.clear();

    startLoad = 0;
    cntLoad = 60;
    isFetch = true;
    beginResetModel();
    endResetModel();

}

SetterOut *SetterModel::GetItem(int row)
{
    return listSetter->GetItemAtRow(row);
}

bool SetterModel::DeleteItem(int row)
{
    return removeRow(row);
}

// bool SetterModel::removeColumns(int column, int count, const QModelIndex &parent)
// {
//     beginRemoveColumns(parent, column, column + count - 1);
//     // FIXME: Implement me!
//     endRemoveColumns();
//     return true;
// }

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void SetterModel::createList()
{
    listSetter = std::make_unique<ListSetter>();
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void SetterModel::AddItem(SetterOut *item)
{
    int row = listSetter->items.size();
    if(listSetter->AddItem(*item))
    // listSetter->items.push_back(*item);
        insertRows(row, 1);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void SetterModel::UpdateItem(int row)
{
    SetterOut* item = listSetter->GetItemAtRow(row);
    if(item != nullptr)
        listSetter->UpdateItem(*item);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void SetterModel::setBaseOff()
{
    isBaseOff = true;
}
