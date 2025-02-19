#ifndef ENTITY_H
#define ENTITY_H

// #include "Items.h"

#include <QDateTime>
#include <QString>
#include <infrastructure/general.h>

class Items;

class IEntity
{
public:
    int id = 0;
    int idParent = 0;
    int idShip = 0;
    QString number;
    QString numberDoc;
    QString name;
    QDateTime dateCreate;
    int idType = 0;

    // QList<IEntity> childItems;
    virtual IndexType GetInfo(QString& /*nameType*/, QString& /*nameIcon*/) const  = 0; //{ return -1;}

    QList<Items> childItems;

private:
};

#endif // ENTITY_H
