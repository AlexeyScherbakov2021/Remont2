#ifndef LISTSHIPMENT_H
#define LISTSHIPMENT_H

#include "listdevice.h"
#include "shipment.h"


class ListShipment //: public ListDevice<Shipment>
{
public:
    explicit ListShipment();

    QList<Shipment> listItems;

    // ListDevice interface
public:
    void LoadChild(Shipment &item);
    void Load() ;
    void getData(int row, int col) const ;
    Shipment GetItem(int id);

    bool DeleteItem(int id);

    void FindItems(const QString &number, int status = 0, bool isFree = false)
    {
        repo.FindItems(number, listItems, status, isFree);
    }

private:
    RepoMSSQL repo;

};

#endif // LISTSHIPMENT_H
