#ifndef LISTPLATE_H
#define LISTPLATE_H

#include "Items.h"
#include "listdevice.h"
// #include <QDebug>

class ListPlate : public ListDevice
{
public:

    explicit ListPlate();

    ~ListPlate() { /*qDebug() << "destructor ListPlate";*/}

    // ListDevice interface
public:
    void GetHeader(QStringList& headers) override;
    // void Load() override;
    QVariant getData(int row, int col, int role) const override;
    // bool DeleteItem(int row) override;


public:
    void FindSerialNumber(const QString &number, QList<Items> &listItems);
    // void LoadChild(Items &plate) override;
    Items GetItem(int id) override;


    void FindItemsExclude(const QString &number, QList<Items> &listExclude, int status = 0)
    {
        ListDevice::FindItems(number, status, true);

        for(auto it : listExclude)
        {
            items.removeIf([it](const Items p) { return p.id == it.id;});
        }
    }

    void setData(int row, int col, const QVariant value, int role) const override;

};

#endif // LISTPLATE_H
