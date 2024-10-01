#ifndef IDEVICE_H
#define IDEVICE_H

#include "entity.h"

class IDevice : public IEntity
{
public:
    int idParent;
    QString name;
    QString number2;
    int idShipment;
    int idType;
    bool isZip;

    explicit IDevice() : idParent(0) {}
};

#endif // IDEVICE_H
