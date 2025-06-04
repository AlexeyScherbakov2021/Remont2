#ifndef MODELVNFT_H
#define MODELVNFT_H

#include "ItemsType.h"

#include <QAbstractListModel>

class ModelVNFT : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ModelVNFT(QObject *parent = nullptr);

    // QVariant headerData(int section,
    //                     Qt::Orientation orientation,
    //                     int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadList(IndexType type);
    ItemType* getType(int row);
    ItemType* searchVNFT(QString VNFT);

private:

    QList<ItemType> listType;

};

#endif // MODELVNFT_H
