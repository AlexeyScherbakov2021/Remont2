#ifndef ITEMSTYPE_H
#define ITEMSTYPE_H

#include <QString>


class ItemType
{

public:
    enum IndexType { Product, Modul, Plate };

    int id;
    IndexType indexType;
    QString typeName;
    int garantMonth;
    QString VNFT;
};

#endif // ITEMSTYPE_H
