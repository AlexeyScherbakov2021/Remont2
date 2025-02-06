#include "listshipment.h"

ListShipment::ListShipment() {}


void ListShipment::LoadChild(Shipment &/*item*/)
{
}

void ListShipment::Load()
{
}

void ListShipment::getData(int /*row*/, int /*col*/) const
{
    return void();
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
