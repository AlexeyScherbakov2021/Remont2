#include "claimmodel.h"

ClaimModel::ClaimModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    createList();
    listClaim->GetHeader(headers);
}

QVariant ClaimModel::headerData(int section, Qt::Orientation orientation, int role) const
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

// bool ClaimModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
// {
//     if (value != headerData(section, orientation, role)) {
//         // FIXME: Implement me!
//         emit headerDataChanged(orientation, section, section);
//         return true;
//     }
//     return false;
// }

int ClaimModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return listClaim->items.size();
}

int ClaimModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.size();
}

// bool ClaimModel::hasChildren(const QModelIndex &parent) const
// {
//     // FIXME: Implement me!
// }

bool ClaimModel::canFetchMore(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return isFetch;
}

void ClaimModel::fetchMore(const QModelIndex &parent)
{
    if(!isBaseOff)
    {
        int resLoad = listClaim->LoadPart(startLoad, cntLoad, number);

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

QVariant ClaimModel::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (!index.isValid())
        return var;

    var = listClaim->getData(index.row(), index.column(), role);

    return var;
}

bool ClaimModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        listClaim->setData(index.row(), index.column(), value, role);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

// Qt::ItemFlags ClaimModel::flags(const QModelIndex &index) const
// {
//     if (!index.isValid())
//         return Qt::NoItemFlags;

//     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
// }

bool ClaimModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

// bool ClaimModel::insertColumns(int column, int count, const QModelIndex &parent)
// {
//     beginInsertColumns(parent, column, column + count - 1);
//     // FIXME: Implement me!
//     endInsertColumns();
//     return true;
// }

bool ClaimModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    listClaim->DeleteItem(row);
    endRemoveRows();
    return true;
}

// bool ClaimModel::removeColumns(int column, int count, const QModelIndex &parent)
// {
//     beginRemoveColumns(parent, column, column + count - 1);
//     // FIXME: Implement me!
//     endRemoveColumns();
//     return true;
// }

void ClaimModel::setData(const QModelIndex &index, Claim *item)
{
    listClaim->setItem(index.row(), item);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ClaimModel::prepareLoad(const QString _number)
{
    number = _number;

    listClaim->items.clear();

    startLoad = 0;
    cntLoad = 60;
    isFetch = true;
    beginResetModel();
    endResetModel();

}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
Claim *ClaimModel::GetItem(int row)
{
    return listClaim->GetItemAtRow(row);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool ClaimModel::DeleteItem(int row)
{
    return removeRow(row);
}


//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ClaimModel::createList()
{
    listClaim = std::make_unique<ListClaim>();
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ClaimModel::AddItem(Claim *item)
{
    int row = listClaim->items.size();
    if(listClaim->AddItem(*item))
        // listSetter->items.push_back(*item);
        insertRows(row, 1);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ClaimModel::UpdateItem(int row)
{
    Claim* item = listClaim->GetItemAtRow(row);
    if(item != nullptr)
        listClaim->UpdateItem(*item);
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
void ClaimModel::setBaseOff()
{
    isBaseOff = true;
}
