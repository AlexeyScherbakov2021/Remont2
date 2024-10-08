#ifndef MODULTYPE_H
#define MODULTYPE_H

#include "entity.h"
#include <QString>


class ModulType : public IEntity
{
public:
    QString name;
    int garantMonth;

    bool operator==(const ModulType &other) const { return this->id == other.id; }
};


inline size_t qHash(const ModulType &key/*, uint seed*/){
    return qHash(key.id);
}
#endif // MODULTYPE_H
