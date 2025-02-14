#ifndef ITEMSTYPE_H
#define ITEMSTYPE_H

#include <QString>
#include <infrastructure/general.h>

class ItemType
{

public:
    // enum IndexType : int { Product, Modul, Plate, Claim, All = 100 };

    int id;
    IndexType indexType;
    QString typeName;
    int garantMonth;
    QString VNFT;
};

#endif // ITEMSTYPE_H
