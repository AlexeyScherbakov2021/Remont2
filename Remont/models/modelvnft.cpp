#include "modelvnft.h"

#include <repomssql.h>

ModelVNFT::ModelVNFT(QObject *parent)
    : QAbstractListModel(parent)
{}

// QVariant ModelVNFT::headerData(int section, Qt::Orientation orientation, int role) const
// {
//     return QVariant();
// }

int ModelVNFT::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return listType.size();
}

//------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------
QVariant ModelVNFT::data(const QModelIndex &index, int role) const
{
    QVariant var;

    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole) && listType.size() > index.row())
        var = listType[index.row()].VNFT + " : " + listType[index.row()].typeName;

    return var;
}

//------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------
void ModelVNFT::loadList(IndexType type)
{
    RepoMSSQL repo;
    repo.LoadNewItemsType(listType, type);
}


//------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------
ItemType *ModelVNFT::getType(int row)
{
    ItemType *ptr = nullptr;

    if(row < listType.size())
        ptr = &listType[row];

    return ptr;
}

//------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------
ItemType* ModelVNFT::searchVNFT(QString VNFT)
{
    ItemType *ptr = nullptr;

    for(auto &it : listType)
    {
        if(it.VNFT == VNFT)
        {
            ptr = &it;
            return ptr;
        }
    }
    return ptr;
}
