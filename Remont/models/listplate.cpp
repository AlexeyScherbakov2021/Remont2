#include "listplate.h"

ListPlate::ListPlate() {}


void ListPlate::Load()
{
}

QVariant ListPlate::getData(int /*row*/, int /*col*/) const
{
    return QVariant();
}

void ListPlate::FindSerialNumber(const QString &number, QList<Plate> &listItems)
{
    repo.FindItems(number, listItems);
}


void ListPlate::LoadChild(Plate &/*plate*/)
{
}


Plate ListPlate::GetItem(int id)
{
    // Plate plate;
    auto plate_it = std::find_if(listItems.cbegin(), listItems.cend(), [id](const Plate &plate) { return plate.id == id;});
    return *plate_it;
}



bool ListPlate::DeleteItem(int id)
{
    return repo.DeletePlate(id);
}
