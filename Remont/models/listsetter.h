#ifndef LISTSETTER_H
#define LISTSETTER_H

#include "listdevice.h"
#include "setterout.h"


class ListSetter //: public ListDevice<SetterOut>
{
public:
    explicit ListSetter();

    QList<SetterOut> listItems;

    // ListDevice interface
public:
    SetterOut GetItem(int id) ;
    void LoadChild(SetterOut &item) ;
    void Load() ;
    QVariant getData(int row, int col) const ;


    bool DeleteItem(int id) ;
private:
    RepoMSSQL repo;

};

#endif // LISTSETTER_H
