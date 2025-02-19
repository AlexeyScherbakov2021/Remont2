#ifndef SETTEROUT_H
#define SETTEROUT_H

#include "Items.h"
#include "entity.h"
// #include "product.h"

// class Modul;
// class Shipment;
// class Product;


class SetterOut : public IEntity
{
public:

    // QList<Items> listItems;

    explicit SetterOut() {}
    virtual IndexType GetInfo(QString& nameType, QString& nameIcon) const
    {
        nameType = "Набор";
        nameIcon = ":/image/setter.png";
        return IndexType::SetterType;
    }

};

#endif // SETTEROUT_H
