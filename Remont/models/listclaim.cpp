#include "listclaim.h"

ListClaim::ListClaim() : ListEntity<Claim>(IndexType::ClaimType)
{
    headers << "Номер" << "Дата" << "Тип" << "Организация" ;
}


bool ListClaim::AddItem(Claim &item)
{
    items.append(item);
    return true;
}

int ListClaim::GetRowFromId(int id)
{
    return 0;
}

bool ListClaim::DeleteItemId(int id)
{
    return repo.DeleteClaim(id);
}

void ListClaim::DeleteItemFromList(int row)
{
}

bool ListClaim::DeleteItem(int row)
{
    bool res = false;
    Claim claim = items.at(row);
    res = repo.DeleteClaim(claim.id);
    if(claim.id > 0 && res)
        items.removeAt(row);
    return res;
}

bool ListClaim::UpdateItem(Claim &item)
{
    return repo.UpdateItem(item);
}

Claim ListClaim::GetItem(int row)
{
    return items.at(row);
    // return Claim();
}

bool ListClaim::LoadChild(Claim &item)
{
    return false;
}

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
            var = claim.TypeClaimString;
            break;
        case 3:
            var = claim.nameOrganization;
            break;
        }
    }

    return var;
}

void ListClaim::setData(int row, int col, const QVariant value, int role) const
{
}

void ListClaim::GetHeader(QStringList &_headers)
{
    _headers = headers;
}

bool ListClaim::Load(const QString Number)
{
    return repo.LoadClaim(Number, items);
}

int ListClaim::getColumnCount()
{
    return headers.size();
}

int ListClaim::getRowCount()
{
    return items.size();
}



Claim ListClaim::GetItemAtRow(int row)
{
    return items.at(row);
}
