#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "entity.h"
// #include "modul.h"
// #include "setterout.h"
// #include "product.h"
#include "Items.h"
#include "organization.h"
#include "setterout.h"

// class SetterOut;

class Shipment : public IEntity
{
public:
    // int isOrganization;
    QString objectInstall;
    QString schet;
    QString cardOrder;
    QDateTime dateUPD;
    QString buyer;
    int idOrganization;
    QDateTime dateRegister;
    QString customer;
    QString questList;
    QString numberUPD;

    Organization org;
    QList<SetterOut> listSetterOut;
    QList<Items> listItems;

    explicit Shipment() {}

    virtual void GetInfo(QString& /*nameType*/, QString& /*nameIcon*/) const {}


};

#endif // SHIPMENT_H
