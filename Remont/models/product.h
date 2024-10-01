#ifndef PRODUCT_H
#define PRODUCT_H

#include "IDevice.h"
#include <infrastructure/IStatus.h>
#include "modul.h"

#define ColumnProduct 13

class Product : public IDevice, public StatusList<Product>
{
public:
    int idSetter;
    QString listQuest;
    QString shunt;

    int idSetterOut;
    int prodTypeId;
    QString redaction1;
    QString redaction2;
    QString redactionPS;
    QString questList;
    bool isAvr;
    bool isAkb;
    bool isCooler;
    bool isSkm;
    bool isZip;
    QString numberBI;
    QString numberUSIKP;

    QList<Modul> listModules;

    explicit Product() {}
};

#endif // PRODUCT_H
