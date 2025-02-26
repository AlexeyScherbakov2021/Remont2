#ifndef CLAIM_H
#define CLAIM_H

// #include "entity.h"
// #include "modul.h"
// #include "product.h"
#include "Items.h"

class Claim : public IEntity
{
public:
    // QString reason;
    // QString remark;

    // QString FromWho;
    int idOrg = 0;
    int idTypeClaim = 0;
    QString nameOrganization;
    QString ObjectInstall;
    QString TypeClaimString;
    bool isClosed = false;

    Claim() {}

    virtual IndexType GetInfo(QString& nameType, QString& nameIcon) const
    {
        nameType = "Рекламация";
        nameIcon = ":/image/claim_open.png";
        return IndexType::ClaimType;
    }


};

#endif // CLAIM_H
