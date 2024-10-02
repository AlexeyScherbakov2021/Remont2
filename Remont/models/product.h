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

    QString numAndComment()
    {
        QString comment = getLastComment().isEmpty() ? "" : " (" + getLastComment() + ")";
        return number + comment;

        // QString res = number;
        // Status status = listStatus.last();
        // if(listStatus.size() > 0 && !status.Comment.isEmpty())
        // {
        //     res = number + " (" + status.Comment + ")";
        // }
        // return res;
    }

};

#endif // PRODUCT_H
