#include "listclaim.h"

ListClaim::ListClaim() {}


Claim ListClaim::GetItem(int /*id*/)
{
    Claim claim;
    return claim;
}

void ListClaim::LoadChild(Claim &/*item*/)
{
}

void ListClaim::Load()
{
    repo.LoadClaim(listItems);
}

QVariant ListClaim::getData(int /*row*/, int /*col*/) const
{
    return QVariant();
}


bool ListClaim::DeleteItem(int id)
{
    return repo.DeleteClaim(id);
}
