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

    bool CheckAndClose()
    {
        bool res = true;
        RepoMSSQL repo;
        repo.LoadChildClaim(*this);

        for(auto &it : childItems)
        {
            repo.LoadStatus(it);
            res &= (it.listStatus.last().idStatus == StatusItem::WORK || it.listStatus.last().idStatus == StatusItem::EXCHANGE
                    /*|| it.listStatus.last().typeStatus == TypeStatus::REMONT_STATUS*/);
        }
        if(res)
        {
            isClosed = true;
            repo.UpdateItem(*this);
        }

        return res;
    }


};

#endif // CLAIM_H
