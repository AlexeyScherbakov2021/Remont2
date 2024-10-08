#include "listsetter.h"

ListSetter::ListSetter() {}


SetterOut ListSetter::GetItem(int /*id*/)
{
    SetterOut setter;
    return setter;
}

void ListSetter::LoadChild(SetterOut &/*item*/)
{
}

void ListSetter::Load()
{
}

QVariant ListSetter::getData(int /*row*/, int /*col*/) const
{
    return QVariant();
}


bool ListSetter::DeleteItem(int id)
{
    return repo.DeleteSetter(id);
}
