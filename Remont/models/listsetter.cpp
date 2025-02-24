#include "listsetter.h"

ListSetter::ListSetter() : ListEntity(IndexType::SetterType)
{

}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListSetter::AddItem(SetterOut &item)
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
int ListSetter::GetRowFromId(int id)
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
bool ListSetter::DeleteItemId(int id)
{
    bool res = repo.DeleteSetter(id);
    if(res)
    {
        items.removeIf([id](const SetterOut& setter) { return setter.id == id;});
    }
    return res;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListSetter::DeleteItemFromList(int row)
{
    if(row < items.size())
        items.removeAt(row);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListSetter::DeleteItem(int row)
{
    bool res = false;
    if(row < items.size())
    {
        res = DeleteItemId(items[row].id);
    }
    return res;
}

bool ListSetter::UpdateItem(SetterOut &item)
{
    bool res = repo.UpdateItem(item);
    return res;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
SetterOut ListSetter::GetItem(int id)
{
    SetterOut setter = repo.GetSetter(id);
    return setter;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
SetterOut* ListSetter::GetItemAtRow(int row)
{
    if(row < items.size())
        return &items[row];
    return nullptr;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListSetter::LoadChild(SetterOut &item)
{
    repo.LoadChildSetter(item);
    return true;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
QVariant ListSetter::getData(int row, int col, int role) const
{
    QVariant var;

    if(role == Qt::DisplayRole)
    {
        SetterOut setter = items.at(row);
        switch(col)
        {
        case 0:
            var = setter.numberDoc;
            break;
        case 1:
            var = setter.dateCreate.toString("dd.MM.yyyy");
            break;
        case 2:
            var = setter.name;
            break;
        }
    }

    return var;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListSetter::setData(int row, int col, const QVariant value, int role) const
{
    if(role == Qt::EditRole)
    {

        SetterOut *setter = const_cast<SetterOut*>(&items[row]);
        switch(col)
        {
        case 0:
            setter->numberDoc = value.toString();
            break;
        case 1:
            setter->dateCreate = value.toDateTime();
            break;
        case 2:
            setter->name = value.toString();
            break;
        }
    }
}

void ListSetter::setItem(int row, SetterOut *setter)
{
    items[row].idShip = setter->idShip;
    items[row].name = setter->name;
    items[row].numberDoc = setter->numberDoc;
    items[row].dateCreate = setter->dateCreate;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListSetter::GetHeader(QStringList &headers)
{
    headers << "Карта заказа" << "Дата" << "Наименование";
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
int ListSetter::LoadPart(int start, int count, const QString &number, bool isFree)
{
    return repo.LoadPart(start, count, number, items, isFree);
}

void ListSetter::AddItemToList(SetterOut *item)
{

}
