#ifndef CLAIM_H
#define CLAIM_H

#include "entity.h"


class Claim : IEntity
{
    int idTypeClaim;
    QString reason;
    QString remark;
};

#endif // CLAIM_H
