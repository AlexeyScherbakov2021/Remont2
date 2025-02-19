#ifndef LISTDEVICE_H
#define LISTDEVICE_H

#include "listentity.h"
// #include <QDebug>
#include <QIcon>
#include <QList>
#include <repomssql.h>
#include "Items.h"


class ListDevice : public ListEntity<Items>
{
// private:
//     IndexType typeDev;
    friend class DeviceModel;


protected:
    // QList<Items> items;
    // IndexType typeDev;

public:
    explicit ListDevice(IndexType _typeDev);

    ~ListDevice() {  }

    virtual bool AddItem(Items &item) override;
    int GetRowFromId(int id) override;
    bool DeleteItemId(int id) override;
    void DeleteItemFromList(int row) override;
    virtual bool DeleteItem(int row) override;
    virtual bool UpdateItem(Items &item) override;
    void FindItems(const QString &number, int status = 0, bool isFree = false);
    Items GetItem(int id) override;
    virtual bool LoadChild(Items &item) override;
    virtual QVariant getData(int row, int col, int role) const override;
    virtual void setData(int row, int col, const QVariant value, int role) const override;
    virtual void GetHeader(QStringList& headers) override;
    int LoadPart2(int start, int count, const QString &number, QVector<StatusItem> vStatus, bool isBusy = false, bool isParent = false) override;
    Items* GetItemAtRow(int row) override;

    // size_t LoadPart(int start, int count, const QString &number, int status = 0, bool isBusy = false, bool isParent = false)
    // {
    //     return repo.LoadPart(start, count, typeDev, number, items, status, isBusy, isParent);
    // }


};


#endif // LISTDEVICE_H
