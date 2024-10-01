#include "productmodel.h"

// template <typename T>
ProductModel::ProductModel(QObject *parent)  : AbstractModel(parent)
{}

// template <typename T>
QVariant ProductModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant var;

    if(role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal)
    {
        switch(section)
        {
        case 0:
            var = "ID";
            break;
        case 1:
            var = "Серийный №";
            break;
        // default:
        //     var = "Column";

        }
    }

    return var;
}

// template <typename T>
bool ProductModel::setHeaderData(int section,
                                 Qt::Orientation orientation,
                                 const QVariant &value,
                                 int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

// template <typename T>
int ProductModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return Products.listItems.size();
}

// template <typename T>
int ProductModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnProduct;
}

// template <typename T>
bool ProductModel::hasChildren(const QModelIndex &parent) const
{
    return false;
}

// bool ProductModel::canFetchMore(const QModelIndex &parent) const
// {
//     return true;
// }

// void ProductModel::fetchMore(const QModelIndex &parent)
// {

// }

// template <typename T>
QVariant ProductModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        // QVariant var = Products.getData(index.row(), index.column());
        return Products.getData(index.row(), index.column());
    }

    return QVariant();
}

// template <typename T>
bool ProductModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

// Qt::ItemFlags ProductModel::flags(const QModelIndex &index) const
// {
//     if (!index.isValid())
//         return Qt::NoItemFlags;

//     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
// }

// template <typename T>
bool ProductModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

// bool ProductModel::insertColumns(int column, int count, const QModelIndex &parent)
// {
//     beginInsertColumns(parent, column, column + count - 1);
//     // FIXME: Implement me!
//     endInsertColumns();
//     return true;
// }

// template <typename T>
bool ProductModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

// template <typename T>
void ProductModel::Load()
{
    Products.Load();
}

// bool ProductModel::removeColumns(int column, int count, const QModelIndex &parent)
// {
//     beginRemoveColumns(parent, column, column + count - 1);
//     // FIXME: Implement me!
//     endRemoveColumns();
//     return true;
// }
