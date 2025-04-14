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
    QString dogovor;

    Organization org;
    QList<SetterOut> listSetterOut;
    // QList<Items> listItems;

    explicit Shipment() {}

    virtual IndexType GetInfo(QString& nameType, QString& nameIcon) const
    {
        nameType = "Отгрузка";
        nameIcon = ":/image/lorry.png";
        return IndexType::ShipType;
    }


};

#endif // SHIPMENT_H
