#include "listplate.h"
#include <QIcon>

ListPlate::ListPlate() : ListDevice(IndexType::Plate) { /*qDebug() << "construct ListPlate";*/}

void ListPlate::GetHeader(QStringList& headers)
{
    headers << "Номер" << "№ прошивки" << "Обозначение" << "Дата регистрации" << "Документ" << "В модуле" << "Статус";
}


// void ListPlate::Load()
// {
// }

// QVariant ListPlate::getData(int row, int col, int role) const
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

// void ListPlate::FindSerialNumber(const QString &number, QList<Items> &listItems)
// {
//     repo.FindItems(IndexType::Plate, number, listItems);
// }


// void ListPlate::LoadChild(Items &/*plate*/)
// {
// }


// Items ListPlate::GetItem(int id)
// {
//     // Plate plate;
//     auto plate_it = std::find_if(items.cbegin(), items.cend(), [id](const Items &plate) { return plate.id == id;});
//     return *plate_it;
// }



// bool ListPlate::DeleteItem(int row)
// {
//     bool res = false;
//     Items plate = items.at(row);
//     if(plate.id != 0)
//     {
//         res = repo.DeleteItem(plate.id);
//         if(res)
//             items.removeAt(row);
//     }
//     return res;
// }


void ListPlate::setData(int row, int col, const QVariant value, int role) const
{
    Items* item = const_cast<Items*>(&items[row]);

    if(role == Qt::EditRole)
    {
        switch(col)
        {
        case 0:
            item->number = value.toString();
            break;
        case 1:
            item->number2 = value.toString();
            break;
        case 2:
            item->VNFT = value.toString();
            break;
        case 3:
            item->dateCreate = value.toDateTime();
            break;
        case 4:
            item->numberDoc = value.toString();
            break;

        // case 6:
        //     item.currStatus = value;
        //     break;

        }
    }
}

