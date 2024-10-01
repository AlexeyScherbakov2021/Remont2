#include "listmodul.h"
#include "modul.h"

ListModul::ListModul() {}


void ListModul::Load()
{
    repo.FindItems(listItems, 0);
}

QVariant ListModul::getData(int /*row*/, int /*col*/) const
{
    return QVariant();
}

bool ListModul::LinkPlate(int idPlate, int idModul)
{
    return repo.LinkPlate(idPlate, idModul);
}


void ListModul::LoadChild(Modul &/*modul*/)
{
}


Modul ListModul::GetItem(int /*id*/)
{
    Modul mod;
    return mod;
}

