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
    // repo.LoadClaim(listItems);
}

void ListClaim::getData(int /*row*/, int /*col*/) const
{
    return void();
}


bool ListClaim::DeleteItem(int id)
{
    return false;
    // return repo.DeleteClaim(id);
}
