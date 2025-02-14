#include "entity.h"
#include "ItemsType.h"

void IEntity::GetInfo(QString &nameType, QString &nameIcon) const
{
    switch(idType)
    {
    case IndexType::Product:
        nameType = "Изделие";
        nameIcon = ":/image/product.png";
        break;
    case IndexType::Modul:
        nameType = "Модуль";
        nameIcon = ":/image/modul.png";
        break;
    case IndexType::Plate:
        nameType = "Плата";
        nameIcon = ":/image/network_adapter.png";
        break;
    case IndexType::All:
        nameType = "Любой";
        // nameIcon = ":/image/network_adapter.png";
        break;
    }
}
