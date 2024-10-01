#ifndef PLATE_H
#define PLATE_H

#include "IDevice.h"
#include <QDateTime>

struct Plate : IDevice
{
    QString VNFT;
    QString numberDoc;

    explicit Plate () {}
};


#endif // PLATE_H
