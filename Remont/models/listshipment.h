#ifndef LISTSHIPMENT_H
#define LISTSHIPMENT_H

#include "listdevice.h"
#include "shipment.h"
// #include "shipmodel.h"


class ListShipment : public ListEntity<Shipment>
{
    friend class ShipModel;

public:
    explicit ListShipment();

    // QList<Shipment> listItems;

public:
    bool AddItem(Shipment &item) override;
    int GetRowFromId(int id) override;
    bool DeleteItemId(int id) override;
    void DeleteItemFromList(int row) override;
    bool DeleteItem(int row) override;
    bool UpdateItem(Shipment &item) override;
    Shipment GetItem(int id) override;
    Shipment* GetItemAtRow(int row) override;
    bool LoadChild(Shipment &item) override;
    QVariant getData(int row, int col, int role) const override;
    void setData(int row, int col, const QVariant value, int role) const override;
    void setItem(int row, Shipment *ship);
    void GetHeader(QStringList &headers) override;
    int LoadPart(int, int, const QString &, bool);
    void AddItemToList(Shipment* item) override;

};

#endif // LISTSHIPMENT_H
