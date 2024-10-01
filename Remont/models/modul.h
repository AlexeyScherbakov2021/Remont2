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

    bool operator==(const Modul &other) const { return this->id == other.id; }

    QString numAndComment()
    {
        QString res = number;
        Status status = listStatus.last();
        if(listStatus.size() > 0 && !status.Comment.isEmpty())
        {
            res = number + " (" + status.Comment + ")";
        }
        return res;
    }


};

inline size_t qHash(const Modul &key/*, uint seed*/){
    return qHash(key.id);
}

#endif // MODUL_H
