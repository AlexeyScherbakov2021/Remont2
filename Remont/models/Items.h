#ifndef ITEMS_H
#define ITEMS_H

#include "ItemsType.h"
#include <QDateTime>
#include <QString>
#include <infrastructure/IStatus.h>

// class StatusList;


class Items : public StatusList
{
public:
    int id;
    int idParent;
    int idShip;
    int idSet;
    int idType;

    QString number;
    QString number2;
    QString numberDoc;
    QString name;
    QDateTime dateCreate;
    QDateTime dateOn;
    QDateTime dateOff;
    int garantMonth;
    QDateTime dateGarant;
    bool isZip;

    ItemType type;
    QList<Items> childItems;

    QString VNFT;
    QString currStatus;

    QString FullNameAndComment()
    {
        QString s = getLastComment();
        if(!s.isEmpty())
            s = "(" + s + ")";
        return name + " №" + number + s;
    }


    // bool operator==(const Items &other) const { return this->id == other.id; }

    // inline size_t qHash(const Items &key/*, uint seed*/){
    //     return qHash(key.id);
    // StatusList listStatus;
};


#endif // ITEMS_H
