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
    virtual void GetInfo(QString& /*nameType*/, QString& /*nameIcon*/) const {}

};

#endif // SETTEROUT_H
