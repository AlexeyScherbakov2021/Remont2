#ifndef MODUL_H
#define MODUL_H

#include "IDevice.h"
#include "plate.h"
#include "modul.h"

#include <QString>
#include <infrastructure/IStatus.h>

class Modul : public IDevice, public StatusList<Modul>
{
public:
    int idProduct;

    QList<IDevice> listChild;
    QList<Plate> listPlate;

    explicit Modul() {}
};
#endif // MODUL_H
