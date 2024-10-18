#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "entity.h"
#include "modul.h"
#include "setterout.h"
#include "product.h"

class Shipment : public IEntity
{
public:
    int isOrganization;
    QString objectInstall;
    QString schet;
    QString cardOrder;
    QString numUPD;
    QDateTime dateUPD;
    QString buyer;
    int idOrganization;
    QDateTime dateRegister;
    QString customer;
    QString questList;
    QString numberUPD;

    QList<Product> listProduct;
    QList<SetterOut> listSetterOut;
    QList<Modul> listModules;

    explicit Shipment() {}

};

#endif // SHIPMENT_H
