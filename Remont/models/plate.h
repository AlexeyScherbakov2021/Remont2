#ifndef PLATE_H
#define PLATE_H

#include "IDevice.h"
#include <QDateTime>
#include <infrastructure/IStatus.h>

class Plate : public IDevice, public StatusList<Plate>
{
public:
    QString VNFT;
    QString numberDoc;

    explicit Plate ()  {}

    // void setKindDevice() override { typeDevice = ev::DeviceKind::PLATE; }

};


#endif // PLATE_H
