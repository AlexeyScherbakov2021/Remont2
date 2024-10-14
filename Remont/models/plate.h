#ifndef PLATE_H
#define PLATE_H

#include "IDevice.h"
#include <QDateTime>

class Plate : public IDevice
{
public:
    QString VNFT;
    QString numberDoc;

    explicit Plate () {}

    // void setKindDevice() override { typeDevice = ev::DeviceKind::PLATE; }

};


#endif // PLATE_H
