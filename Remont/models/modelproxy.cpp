#include "modelproxy.h"
#include <QDebug>

ModelProxy::ModelProxy(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

void ModelProxy::AddExcludeListId(QSet<int> _listId)
{
    listId = _listId;
}


bool ModelProxy::filterAcceptsRow(int source_row, const QModelIndex &/*source_parent*/) const
{
    const QModelIndex ind = sourceModel()->index(source_row, 0);
    int id = sourceModel()->data(ind, Qt::UserRole + 10).toInt();
    // qDebug() << id;
    return !listId.contains(id);
    // return true;
}
