#ifndef ENTITY_H
#define ENTITY_H

#include <QDateTime>
#include <QString>

class IEntity
{
public:
    int id;
    QString number;

    explicit IEntity() : id(0) {}
private:
};

#endif // ENTITY_H
