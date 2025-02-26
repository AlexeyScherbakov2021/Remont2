#include "listclaim.h"

ListClaim::ListClaim() : ListEntity<Claim>(IndexType::ClaimType)
{
    // headers << "Номер" << "Дата" << "Тип" << "Организация" ;
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListClaim::AddItem(Claim &item)
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
int ListClaim::GetRowFromId(int id)
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
bool ListClaim::DeleteItemId(int id)
{
    bool res = repo.DeleteClaim(id);
    if(res)
    {
        items.removeIf([id](const Claim& claim) { return claim.id == id;});
    }
    return res;
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListClaim::DeleteItemFromList(int row)
{
    if(row < items.size())
        items.removeAt(row);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListClaim::DeleteItem(int row)
{
    bool res = false;
    if(row < items.size())
    {
        res = DeleteItemId(items[row].id);
    }
    return res;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListClaim::UpdateItem(Claim &item)
{
    bool res = repo.UpdateItem(item);
    return res;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
Claim ListClaim::GetItem(int id)
{
    Claim claim = repo.GetClaim(id);
    return claim;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
Claim* ListClaim::GetItemAtRow(int row)
{
    if(row < items.size())
        return &items[row];
    return nullptr;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool ListClaim::LoadChild(Claim &item)
{
    repo.LoadChildClaim(item);
    return true;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
QVariant ListClaim::getData(int row, int col, int role) const
{
    QVariant var;

    if(role == Qt::DisplayRole && row < items.size())
    {
        Claim claim = items.at(row);

        switch(col)
        {
        case 0:
            var = claim.number;
            break;
        case 1:
            var = claim.dateCreate.toString("dd.MM.yyyy");
            break;
        case 2:
            var = claim.nameOrganization;
            break;
        case 3:
            var = claim.ObjectInstall;
            break;
        case 4:
            var = claim.TypeClaimString;
            break;
        }
    }

    return var;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListClaim::setData(int row, int col, const QVariant value, int role) const
{
    if(role == Qt::EditRole)
    {

        Claim *claim = const_cast<Claim*>(&items[row]);
        switch(col)
        {
        case 0:
            claim->number = value.toString();
            break;
        case 1:
            claim->dateCreate = value.toDateTime();
            break;
        case 2:
            claim->nameOrganization = value.toString();
            break;
        case 3:
            claim->ObjectInstall = value.toString();
            break;
        case 4:
            claim->TypeClaimString = value.toString();
            break;
        }
    }
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListClaim::setItem(int row, Claim *claim)
{
    items[row].idShip = claim->idShip;
    items[row].name = claim->name;
    items[row].numberDoc = claim->numberDoc;
    items[row].dateCreate = claim->dateCreate;
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListClaim::GetHeader(QStringList &_headers)
{
    _headers << "Номер" << "Дата" << "Организация" << "Объект установки" << "Тип";
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
int ListClaim::LoadPart(int start, int count, const QString &number)
{
    return repo.LoadPart(start, count, number, items);
}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void ListClaim::AddItemToList(Claim */*item*/)
{

}
