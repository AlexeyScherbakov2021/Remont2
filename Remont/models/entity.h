#ifndef ENTITY_H
#define ENTITY_H

// #include "Items.h"

#include <QDateTime>
#include <QString>

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
    virtual void GetInfo(QString& /*nameType*/, QString& /*nameIcon*/) const {}

    QList<Items> childItems;

private:
};

#endif // ENTITY_H
