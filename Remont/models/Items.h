#ifndef ITEMS_H
#define ITEMS_H

// #include "ItemsType.h"
#include "entity.h"
#include <QDateTime>
#include <QString>
#include <infrastructure/IStatus.h>


class Items : public IEntity, public StatusList
{
public:
    // int id = 0;
    // int idParent = 0;
    // int idShip = 0;
    int idSet = 0;
    // int idType = 0;

    // QString number;
    QString number2;
    // QString numberDoc;
    // QString name;
    // QDateTime dateCreate;
    QDateTime dateOn;
    QDateTime dateOff;
    int garantMonth = 0;
    QDateTime dateGarant;
    bool isZip = false;

    ItemType type;
    QList<Items> childItems;

    QString VNFT;
    QString currStatus;
    QString commentStatus;

    QString GetDefaultName()
    {
        return number + " (" + type.typeName + " " + type.VNFT + ")";
    }


    QString FullNameAndComment()
    {
        QString s = getLastComment();
        if(!s.isEmpty())
            s = "(" + s + ")";
        return name + " №" + number + s;
    }

    void GetInfo(QString &nameType, QString &nameIcon) const override
    {
        switch(type.indexType)
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

    // bool operator==(const Items &other) const { return this->id == other.id; }

    // inline size_t qHash(const Items &key/*, uint seed*/){
    //     return qHash(key.id);
    // StatusList listStatus;
};



// class ItemsProduct : public Items
// {
// public:
//     void GetInfo(QString &nameType, QString &nameIcon) override
//     {
//         nameType = "Изделие";
//         nameIcon = ":/image/product.png";
//     }
// };

// class ItemsModul : public Items
// {
// public:
//     void GetInfo(QString &nameType, QString &nameIcon) override
//     {
//         nameType = "Модуль";
//         nameIcon = "://image/modul.png";
//     }
// };

// class ItemsPlate : public Items
// {
// public:
//     void GetInfo(QString &nameType, QString &nameIcon) override
//     {
//         nameType = "Плата";
//         nameIcon = "://image/network_adapter.png";
//     }
// };


#endif // ITEMS_H
