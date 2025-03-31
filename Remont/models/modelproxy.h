#ifndef MODELPROXY_H
#define MODELPROXY_H

#include <QSortFilterProxyModel>

class ModelProxy : public QSortFilterProxyModel
{
public:
    explicit ModelProxy(QObject *parent = nullptr);


    void AddExcludeListId(QSet<int> _listId);
    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QSet<int> listId;
};

#endif // MODELPROXY_H
