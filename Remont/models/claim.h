#ifndef CLAIM_H
#define CLAIM_H

#include "entity.h"
// #include "modul.h"
// #include "product.h"
#include "Items.h"

class Claim : public IEntity
{
public:
    int idTypeClaim;
    QString reason;
    QString remark;

    QString FromWho;
    int idOrg;
    QString nameOrganization;
    QString ObjectInstall;
    QString Descript;
    int TypeComplectId;
    QString VNFT;
    int Quantity;
    int TypeDeviceId;
    QString NumberModul;
    QString NumberNewModul;
    QString NumberDevice;
    QDateTime DateOut;
    bool IsGuarantee;
    QString Reason;
    QDateTime DateRepair;
    QString DoRepair;
    QString FileAnswer;
    QString TextResult;
    QString TypeClaimString;
    QString TypeComplectString;
    QString TypeDeviceString;
    QDateTime dateRegister;


    QList<Items> listModul;
    QList<Items> listProduct;

    Claim() {}

};

#endif // CLAIM_H
