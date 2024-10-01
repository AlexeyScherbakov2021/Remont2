#ifndef LISTSETTER_H
#define LISTSETTER_H

#include "listdevice.h"
#include "setterout.h"


class ListSetter : public ListDevice<SetterOut>
{
public:
    explicit ListSetter();



    // ListDevice interface
public:
    SetterOut GetItem(int id) override;
    void LoadChild(SetterOut &item) override;
    void Load() override;
    QVariant getData(int row, int col) const override;

};

#endif // LISTSETTER_H
