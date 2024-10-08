#ifndef PRODTYPE_H
#define PRODTYPE_H

#include "entity.h"
#include <QString>


class ProductType : public IEntity
{
public:
    QString name;
    int garantMonth;

    bool operator==(const ProductType &other) { return other.id == this->id;}
};

inline size_t qHash(const ProductType &key/*, uint seed*/){
    return qHash(key.id);
}
#endif // PRODTYPE_H
