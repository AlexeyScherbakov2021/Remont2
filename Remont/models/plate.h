#ifndef PLATE_H
#define PLATE_H

#include "IDevice.h"
#include "platetype.h"
#include <QDateTime>
#include <infrastructure/IStatus.h>

class Plate : public IDevice, public StatusList<Plate>
{
public:
    QString VNFT;
    QString numberDoc;
    PlateType pType;

    explicit Plate ()  {}

    // void setKindDevice() override { typeDevice = ev::DeviceKind::PLATE; }
    QString FullNameAndComment()
    {
        QString s = getLastComment();
        if(!s.isEmpty())
            s = "(" + s + ")";
        return name + " №" + number + s;

        // QString res = name + " №" + number;
        // if(listStatus.size() > 0)
        // {
        //     Status status = listStatus.last();
        //     if(!status.Comment.isEmpty())
        //         res = res + " (" + status.Comment + ")";
        // }
        // return res;
    }

};


#endif // PLATE_H
