#include "listproduct.h"

ListProduct::ListProduct() {}

void ListProduct::Load()
{
    repo.FindItems("", listItems, 0);
}

QVariant ListProduct::getData(int row, int col) const
{
    QVariant var;
    switch(col)
    {
    case 0:
        var.setValue(listItems[row].id);
        break;
    case 1:
        var.setValue(listItems[row].number);
        break;
    case 2:
        var.setValue(listItems[row].name);
    }

    return var;
}

void ListProduct::LoadChild(Product &prod)
{
    repo.LoadChildProduct(prod);
}


Product ListProduct::GetItem(int /*id*/)
{
    Product prod;
    return prod;
}



bool ListProduct::DeleteItem(int id)
{
    return repo.DeleteProduct(id);
}
