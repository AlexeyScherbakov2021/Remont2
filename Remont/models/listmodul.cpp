#include "listmodul.h"

#include <QIcon>
// #include "modul.h"

ListModul::ListModul() : ListDevice(ItemType::Modul) {}


// void ListModul::Load()
// {
//     repo.FindItems(ItemType::Product, "", items, 0);
// }

QVariant ListModul::getData(int row, int col, int role) const
{
    QVariant var;

    if(row > items.size())
        return var;

    auto item = items.at(row);

    if(role == Qt::DisplayRole)
    {
        switch(col)
        {
        case 0:
            var = item.number;
            break;
        case 1:
            var = item.number2;
            break;
        case 2:
            var = item.VNFT;
            break;
        case 3:
            var = item.dateCreate.toString("dd.MM.yyyy");
            break;
        case 4:
            var = item.numberDoc;
            break;
        case 6:
            var = item.currStatus;
            break;
        }

    }
    else if(role == Qt::DecorationRole)
    {
        if(col == 5)
        {
            if(item.idParent > 0)
                var = QIcon("://image/Apply24x24.png");
        }
    }

    return var;
}

// bool ListModul::LinkPlate(int idPlate, int idModul)
// {
//     return repo.LinkPlate(idPlate, idModul);
// }


// void ListModul::LoadChild(Items &/*modul*/)
// {
// }


// Modul ListModul::GetItem(int /*id*/)
// {
//     Modul mod;
//     return mod;
// }



// bool ListModul::DeleteItem(int id)
// {
//     return repo.DeleteItem(id);
// }


void ListModul::GetHeader(QStringList &headers)
{
    headers << "Номер" << "№ прошивки" << "Обозначение" << "Дата регистрации" << "Документ" << "В модуле" << "Статус";
}


void ListModul::setData(int /*row*/, int /*col*/, const QVariant /*value*/, int /*role*/) const
{
}


