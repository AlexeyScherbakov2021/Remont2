#ifndef LISTCLAIM_H
#define LISTCLAIM_H

#include "claim.h"
#include "listdevice.h"


class ListClaim : public ListDevice<Claim>
{
public:
    ListClaim();

    // ListDevice interface
public:
    Claim GetItem(int id) override;
    void LoadChild(Claim &item) override;
    void Load() override;
    QVariant getData(int row, int col) const override;

    bool DeleteItem(int id) override;
};

#endif // LISTCLAIM_H
