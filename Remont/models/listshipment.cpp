#include "listshipment.h"

ListShipment::ListShipment() {}


void ListShipment::LoadChild(Shipment &/*item*/)
{
}

void ListShipment::Load()
{
}

QVariant ListShipment::getData(int /*row*/, int /*col*/) const
{
    return QVariant();
}


Shipment ListShipment::GetItem(int id)
{
    return repo.GetShipment(id);
    // return false;
}



bool ListShipment::DeleteItem(int id)
{
    // return repo.DeleteShipment(id);
    return false;

}
