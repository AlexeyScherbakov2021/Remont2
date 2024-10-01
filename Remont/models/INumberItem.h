#ifndef INUMBERITEM_H
#define INUMBERITEM_H

#include "entity.h"


class INumberItem : public IEntity
{
public:
    int idParent;
    QString name;
    QString number2;
};


#endif // INUMBERITEM_H
