#include "listdevice.h"

ListDevice::ListDevice(IndexType _typeDev) : ListEntity<Items>(_typeDev)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

bool ListDevice::AddItem(Items &item)
{
    return repo.AddItem(item);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ListDevice::GetRowFromId(int id)
{
    for(int row = 0; row < items.size(); ++row)
    {
        if(items[row].id == id)
            return row;
    }

    return -1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ListDevice::DeleteItemId(int id)
{
    return repo.DeleteItem(id);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListDevice::DeleteItemFromList(int row)
{
    items.removeAt(row);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ListDevice::DeleteItem(int row)
{
    bool res = false;
    Items item = items.at(row);
    if(item.id != 0)
    {
        res = repo.DeleteItem(item.id);
        if(res)
            items.removeAt(row);
    }
    return res;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ListDevice::UpdateItem(Items &item)
{
    return repo.UpdateItem(item);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// void ListDevice::FindItems(const QString &/*number*/, int /*status*/, bool /*isFree*/)
// {
//     // repo.FindItems(typeDev, number, items, status, isFree);
// }

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Items ListDevice::GetItem(int id)
{
    Items resT;
    auto res = std::find_if(items.cbegin(), items.cend(), [id] (auto it) { return it.id == id; });
    if(res != items.cend())
        resT = *res;
    return resT;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ListDevice::LoadChild(Items &item)
{
    return repo.LoadChildItems(item.id, item.childItems);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
QVariant ListDevice::getData(int row, int col, int role) const
{
    QVariant var;

    if(row > items.size())
        return var;

    auto item = items.at(row);

    if(role == Qt::DisplayRole)
    {
        switch(col)
        {
        case 1:
            var = item.number;
            break;
        case 2:
            var = item.type.typeName;
            break;
        case 3:
            var = item.type.VNFT;
            break;
        case 4:
            var = item.name;
            break;
        case 5:
            var = item.dateCreate.toString("dd.MM.yyyy");
            break;
        case 6:
            var = item.currStatus;
            break;
        }

    }
    else if(role == Qt::DecorationRole)
    {
        if(col == 0)
        {
            QString nameType;
            QString nameIcon;
            item.GetInfo(nameType, nameIcon);
            var = QIcon(nameIcon);
        }
    }
    return var;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListDevice::setData(int, int, const QVariant, int) const
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ListDevice::GetHeader(QStringList &headers)
{
    headers << "" << "Номер" << "Тип" << "Обозначение" << "Наименование" << "Дата рег." << "Статус";
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ListDevice::LoadPart(int start, int count, const QString &number, QVector<StatusItem> vStatus, bool isBusy, LoadPartType hasParent)
{
    if(typeDev == IndexType::All)
        return repo.LoadPartAll(start, count, number, items, vStatus, isBusy, hasParent);
    else
        return repo.LoadPart(start, count, typeDev, number, items, vStatus, isBusy, hasParent);
}




Items* ListDevice::GetItemAtRow(int row)
{
    if(row < items.size())
        return &items[row];

    return nullptr;
}

void ListDevice::AddItemToList(Items *item)
{
    items.push_back(*item);
}
