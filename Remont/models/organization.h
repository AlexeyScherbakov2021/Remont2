#ifndef ORGANIZATION_H
#define ORGANIZATION_H

#include <QString>


class Organization
{
public:
    int id;
    QString UID;
    QString orgName;
    QString INN;
    QString KPP;

    Organization() : id(0) {}

    const QString getFullName() const
    {
        return orgName + " (ИНН " + INN + " КПП" + KPP + ")";
    }
};

#endif // ORGANIZATION_H
