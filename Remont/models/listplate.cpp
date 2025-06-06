#include "listplate.h"
#include <QIcon>

ListPlate::ListPlate() : ListDevice(IndexType::Plate) { /*qDebug() << "construct ListPlate";*/}

void ListPlate::GetHeader(QStringList& headers)
{
    headers << "" << "Номер" << "№ прошивки" << "Тип" << "Обозначение" << "Дата регистрации" << "Документ" << "Статус";
}


// void ListPlate::Load()
// {
// }

QVariant ListPlate::getData(int row, int col, int role) const
{
    QVariant var;

    if(row > items.size())
        return var;

    auto item = items.at(row);


    if(role == Qt::UserRole + 10)
    {
        var = item.id;
    }

    if(role == Qt::DisplayRole)
    {
        switch(col)
        {
        case 1:
            var = item.number;
            break;
        case 2:
            var = item.number2;
            break;
        case 3:
            var = item.type.typeName;
            break;
        case 4:
            var = item.VNFT;
            break;
        case 5:
            var = item.dateCreate.toString("dd.MM.yyyy");
            break;
        case 6:
            var = item.numberDoc;
            break;
        case 7:
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
        if(col == 7)
        {
            if(item.idParent > 0)
                var = QIcon("://image/Apply24x24.png");
        }
    }

    return var;
}

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
        case 1:
            item->number = value.toString();
            break;
        case 2:
            item->number2 = value.toString();
            break;
        // case 2:
        //     item-> = value.toString();
        //     break;
        case 4:
            item->VNFT = value.toString();
            break;
        case 5:
            item->dateCreate = value.toDateTime();
            break;
        case 6:
            item->numberDoc = value.toString();
            break;

        // case 6:
        //     item.currStatus = value;
        //     break;

        }
    }
}

