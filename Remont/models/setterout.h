#ifndef SETTEROUT_H
#define SETTEROUT_H

#include "entity.h"
#include "product.h"

// class Modul;
// class Shipment;
class Product;


class SetterOut : public IEntity
{
public:
    int idShipment;
    QString name;
    QString orderNumber;
    QList<Product> listProduct;

    explicit SetterOut() : idShipment{0} {}

};

#endif // SETTEROUT_H
