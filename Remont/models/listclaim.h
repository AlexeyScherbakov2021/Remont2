#ifndef LISTCLAIM_H
#define LISTCLAIM_H

#include "claim.h"
#include "listdevice.h"


class ListClaim //: public ListDevice<Claim>
{
public:
    ListClaim();

    QList<Claim> listItems;
    Claim GetItem(int id) ;
    void LoadChild(Claim &item) ;
    void Load() ;
    void getData(int row, int col) const ;
    bool DeleteItem(int id);

private:
    RepoMSSQL repo;

};

#endif // LISTCLAIM_H
