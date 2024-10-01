#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include "entity.h"

#include <QAbstractTableModel>

class AbstractModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit AbstractModel(QObject *parent = nullptr);

    // Header:
    virtual QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    virtual bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant &value,
                       int role = Qt::EditRole) override;

    // Basic functionality:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QList<IEntity> items;
};

#endif // ABSTRACTMODEL_H
