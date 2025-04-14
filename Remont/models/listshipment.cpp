#include "listshipment.h"

ListShipment::ListShipment() : ListEntity(IndexType::ShipType) {}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListShipment::AddItem(Shipment &item)
{
    bool res = repo.AddItem(item);
    if(res)
    {
        items.push_back(item);
    }
    return res;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
int ListShipment::GetRowFromId(int id)
{
    for(int row = 0; row < items.size(); ++row)
    {
        if(items[row].id == id)
            return row;
    }
    return -1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListShipment::DeleteItemId(int id)
{
    bool res = repo.DeleteShipment(id);
    if(res)
    {
        items.removeIf([id](const Shipment& ship) { return ship.id == id;});
    }
    return res;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListShipment::DeleteItemFromList(int row)
{
    if(row < items.size())
        items.removeAt(row);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListShipment::DeleteItem(int row)
{
    bool res = false;
    if(row < items.size())
    {
        res = DeleteItemId(items[row].id);
    }
    return res;
}

bool ListShipment::UpdateItem(Shipment &item)
{
    bool res = repo.UpdateItem(item);
    return res;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
Shipment ListShipment::GetItem(int id)
{
    Shipment ship = repo.GetShipment(id);
    return ship;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
Shipment* ListShipment::GetItemAtRow(int row)
{
    if(row < items.size() && row >= 0)
        return &items[row];
    return nullptr;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListShipment::LoadChild(Shipment &item)
{
    repo.LoadChildShip(item);
    return true;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
QVariant ListShipment::getData(int row, int col, int role) const
{
    QVariant var;

    if(role == Qt::DisplayRole)
    {
        Shipment ship = items.at(row);
        switch(col)
        {
        case 0:
            var = ship.schet;
            break;
        case 1:
            var = ship.org.orgName;
            break;
        case 2:
            var = ship.cardOrder;
            break;
        case 3:
            var = ship.objectInstall;
            break;
        case 4:
            var = ship.numberUPD;
            break;
        case 5:
            var = ship.dateUPD.toString("dd.MM.yyyy");
            break;
        case 6:
            var = ship.dateRegister.toString("dd.MM.yyyy");
            break;
        case 7:
            var = ship.dogovor;
            break;
        }
    }

    return var;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListShipment::setData(int row, int col, const QVariant value, int role) const
{
    if(role == Qt::EditRole)
    {

        Shipment *ship = const_cast<Shipment*>(&items[row]);
        switch(col)
        {
        case 0:
            ship->schet = value.toString();
            break;
        case 1:
            ship->customer = value.toString();
            break;
        case 2:
            ship->cardOrder = value.toString();
            break;
        case 3:
            ship->objectInstall = value.toString();
            break;
        case 4:
            ship->numberUPD = value.toString();
            break;
        case 5:
            ship->dateUPD = value.toDateTime();
            break;
        case 7:
            ship->dogovor = value.toString();
            break;
        }
    }
}

void ListShipment::setItem(int row, Shipment *ship)
{
    items[row].schet = ship->schet;
    items[row].customer = ship->customer;
    items[row].cardOrder = ship->cardOrder;
    items[row].objectInstall = ship->objectInstall;
    items[row].numberUPD = ship->numberUPD;
    items[row].dateUPD = ship->dateUPD;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListShipment::GetHeader(QStringList &headers)
{
    headers << "Счет" << "Заказчик" << "Карта заказа" << "Объект установки" << "№ реализ."
            << "Дата рализ." << "Дата отгр." << "Договор";
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
int ListShipment::LoadPart(int start, int count, const QString &number, bool isShip)
{
    return repo.LoadPart(start, count, number, items, isShip);
}

void ListShipment::AddItemToList(Shipment *item)
{
    items.push_back(*item);

}
