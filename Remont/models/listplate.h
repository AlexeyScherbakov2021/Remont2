#ifndef LISTPLATE_H
#define LISTPLATE_H

#include "listdevice.h"
#include "plate.h"

class ListPlate : public ListDevice<Plate>
{
public:

    explicit ListPlate();

    // ListDevice interface
public:
    void Load() override;
    QVariant getData(int row, int col) const override;
    void FindSerialNumber(const QString &number, QList<Plate> &listItems);
public:
    void LoadChild(Plate &plate) override;
    Plate GetItem(int id) override;

    bool DeleteItem(int id) override;

    void FindItemsExclude(const QString &number, QList<Plate> &listExclude, int status = 0)
    {
        ListDevice::FindItems(number, status);

        for(auto it : listExclude)
        {
            listItems.removeIf([it](const Plate p) { return p.id == it.id;});
        }
    }

};

#endif // LISTPLATE_H
