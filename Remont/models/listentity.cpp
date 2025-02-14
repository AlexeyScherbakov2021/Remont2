#include "listentity.h"

// #include "Items.h"


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// template<class T>
// ListEntity<T>::ListEntity(IndexType _typeDev) : typeDev(_typeDev)
// {
// }


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// template<class T>
// bool ListEntity<T>::AddItem(T &item)
// {
//     return false;
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// int ListEntity<T>::GetRowFromId(int id)
// {
//     for(int row = 0; row < items.size(); ++row)
//     {
//         if(items[row].id == id)
//             return row;
//     }

//     return -1;
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// bool ListEntity<T>::DeleteItemId(int id)
// {
//     // return repo.DeleteItem(id);
//     return false;
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// void ListEntity<T>::DeleteItemFromList(int row)
// {
//     items.removeAt(row);
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// bool ListEntity<T>::DeleteItem(int row)
// {
//     bool res = false;
//     IEntity item = items.at(row);
//     if(item.id != 0)
//     {
//         res = repo.DeleteItem(item.id);
//         if(res)
//             items.removeAt(row);
//     }
//     return res;
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// bool ListEntity<T>::UpdateItem(T&item)
// {
//     // return repo.UpdateItem(item);
//     return false;
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// T ListEntity<T>::GetItem(int id)
// {
//     T resT;
//     auto res = std::find_if(items.cbegin(), items.cend(), [id] (auto it) { return it.id == id; });
//     if(res != items.cend())
//         resT = *res;
//     return resT;
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// bool ListEntity<T>::LoadChild(T &item)
// {
//     // return repo.LoadChildItems(item.id, item.childItems);
//     return false;
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// QVariant ListEntity<T>::getData(int row, int col, int role) const
// {
//     QVariant var;

//     // if(row > items.size())
//     //     return var;

//     // auto item = items.at(row);

//     // if(role == Qt::DisplayRole)
//     // {
//     //     switch(col)
//     //     {
//     //     case 1:
//     //         var = item.number;
//     //         break;
//     //     case 2:
//     //         var = item.VNFT;
//     //         break;
//     //     case 3:
//     //         var = item.name;
//     //         break;
//     //     case 4:
//     //         var = item.dateCreate.toString("dd.MM.yyyy");
//     //         break;
//     //     case 5:
//     //         var = item.numberDoc;
//     //         break;
//     //     case 6:
//     //         var = item.currStatus;
//     //         break;
//     //     }

//     // }
//     // else if(role == Qt::DecorationRole)
//     // {
//     //     if(col == 0)
//     //     {
//     //         QString nameType;
//     //         QString nameIcon;
//     //         item.GetInfo(nameType, nameIcon);
//     //         var = QIcon(nameIcon);
//     //     }
//     // }
//     return var;
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// void ListEntity<T>::setData(int, int, const QVariant, int) const
// {

// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// void ListEntity<T>::GetHeader(QStringList &headers)
// {
//     headers << "Тип" << "Номер" << "Наименование" << "Дата регистрации" << "Документ";
// }

// //-----------------------------------------------------------------------------
// //
// //-----------------------------------------------------------------------------
// template<class T>
// int ListEntity<T>::LoadPart2(int start, int count, const QString &number, QVector<int> vStatus, bool isBusy, bool isParent)
// {
//     // if(typeDev == ItemType::All)
//     //     return repo.LoadPartAll(start, count, number, items, vStatus, isBusy, isParent);
//     // else
//     //     return repo.LoadPart(start, count, typeDev, number, items, vStatus, isBusy, isParent);
//     return 0;
// }

