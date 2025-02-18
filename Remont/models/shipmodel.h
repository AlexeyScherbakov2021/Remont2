#ifndef SHIPMODEL_H
#define SHIPMODEL_H

#include "listsetter.h"
#include "listshipment.h"
// #include "shipment.h"
#include <QAbstractTableModel>

class Shipment;
// class ListShipment;

class ShipModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ShipModel(QObject *parent = nullptr);

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void prepareLoad(const QString _number, bool _isBusy = false);
    Shipment* GetItem(int row);
    bool DeleteItem(int row);
    void DeleteItemFromListId(int id) ;
    void DeleteItemFromList(int row) ;

    void createList();

    void AddItem(Shipment* item);
    void UpdateItem(int row);
    void setBaseOff();

private:
    int startLoad = 0;
    int cntLoad = 60;
    bool isFetch = false;
    QStringList headers;

    std::unique_ptr<ListShipment> listShip;

    QString number = "";
    bool isShip = false;
    bool isBaseOff = false;


};

#endif // SHIPMODEL_H
