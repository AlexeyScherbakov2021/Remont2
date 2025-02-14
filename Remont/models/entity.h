#ifndef ENTITY_H
#define ENTITY_H

#include <QDateTime>
#include <QString>

class IEntity
{
public:
    int id = 0;
    int idParent = 0;
    int idShip = 0;
    QString number;
    QString numberDoc;
    QString name;
    QDateTime dateCreate;
    int idType = 0;

    // QList<IEntity> childItems;
    virtual void GetInfo(QString& nameType, QString& nameIcon) const;

private:
};

#endif // ENTITY_H
