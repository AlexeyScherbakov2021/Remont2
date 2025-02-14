#include "listproduct.h"

#include <QIcon>

ListProduct::ListProduct() : ListDevice(IndexType::Product) {}

// void ListProduct::Load()
// {
//     repo.FindItems(ItemType::Product, "", items, 0);
// }

// QVariant ListProduct::getData(int row, int col, int role) const
// {
//     QVariant var;

//     if(row > items.size())
//         return var;

//     auto item = items.at(row);

//     if(role == Qt::DisplayRole)
//     {
//         switch(col)
//         {
//         case 0:
//             var = item.number;
//             break;
//         case 1:
//             var = item.number2;
//             break;
//         case 2:
//             var = item.VNFT;
//             break;
//         case 3:
//             var = item.dateCreate.toString("dd.MM.yyyy");
//             break;
//         case 4:
//             var = item.numberDoc;
//             break;
//         case 6:
//             var = item.currStatus;
//             break;
//         }

//     }
//     else if(role == Qt::DecorationRole)
//     {
//         if(col == 5)
//         {
//             if(item.idParent > 0)
//                 var = QIcon("://image/Apply24x24.png");
//         }
//     }

//     return var;
// }

// void ListProduct::LoadChild(Items &prod)
// {
//     // repo.LoadChildProduct(prod);
// }


// Product ListProduct::GetItem(int /*id*/)
// {
//     Product prod;
//     return prod;
// }



// bool ListProduct::DeleteItem(int id)
// {
//     return repo.DeleteItem(id);
// }


void ListProduct::GetHeader(QStringList &headers)
{
    headers << "Номер" << "№ прошивки" << "Обозначение" << "Дата регистрации" << "Документ" << "В модуле" << "Статус";
}


void ListProduct::setData(int /*row*/, int /*col*/, const QVariant /*value*/, int /*role*/) const
{
}


