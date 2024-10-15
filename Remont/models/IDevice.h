#ifndef IDEVICE_H
#define IDEVICE_H

#include "entity.h"
#include "enumvariable.h"

class IDevice : public IEntity
{
public:
    int idParent;
    QString name;
    QString number2;
    int idShipment;
    int idType;
    bool isZip;
    QDateTime EndGarant;
    QDateTime dateRegister;
    QDateTime dateOn;
    QDateTime dateUtil;
    int garantMonth;
    int typeDevice;

    explicit IDevice() : idParent(0) { }
// protected:
    // virtual void setKindDevice() = 0;
};

#endif // IDEVICE_H
