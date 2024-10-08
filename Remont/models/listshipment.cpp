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
}



bool ListShipment::DeleteItem(int id)
{
    return repo.DeleteShipment(id);
}
