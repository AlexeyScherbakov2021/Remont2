#ifndef LISTSHIPMENT_H
#define LISTSHIPMENT_H

#include "listdevice.h"
#include "shipment.h"


class ListShipment : public ListDevice<Shipment>
{
public:
    explicit ListShipment();

    // ListDevice interface
public:
    void LoadChild(Shipment &item) override;
    void Load() override;
    QVariant getData(int row, int col) const override;
    Shipment GetItem(int id) override;
};

#endif // LISTSHIPMENT_H
