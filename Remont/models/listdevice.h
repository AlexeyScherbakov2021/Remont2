#ifndef LISTDEVICE_H
#define LISTDEVICE_H

#include "Items.h"
// #include <QDebug>
#include <QList>
#include <repomssql.h>



class ListDevice
{
private:
    ItemType::IndexType typeDev;

public:
    QList<Items> items;

    explicit ListDevice(ItemType::IndexType _typeDev) : typeDev(_typeDev) {}
    ~ListDevice() { /*qDebug() << "destructor ListDevice";*/ }

    virtual void GetHeader(QStringList& headers) = 0;


    virtual bool AddItem(Items &item)
    {
        return repo.AddItem(item);
    }

    virtual bool DeleteItem(int id) = 0;

    virtual bool UpdateItem(Items &item)
    {
        return repo.UpdateItem(item);
    }

    void FindItems(const QString &number, int status = 0, bool isFree = false)
    {
        repo.FindItems(typeDev, number, items, status, isFree);
    }


    virtual Items GetItem(int id)
    {
        Items resT;
        auto res = std::find_if(items.cbegin(), items.cend(), [id] (auto it) { return it.id == id; });
        if(res != items.cend())
            resT = *res;
        return resT;
    }

    virtual Items GetItem(QString number)
    {
        Items resT;
        auto res = std::find_if(items.cbegin(), items.cend(), [number] (auto it) { return it.number == number; });
        if(res != items.cend())
            resT = *res;
        return resT;
    }


    virtual void LoadChild(Items &item) = 0;


    virtual void Load() = 0;

    virtual QVariant getData(int row, int col, int role) const = 0;
    virtual void setData(int row, int col, const QVariant value, int role) const = 0;

    // size_t LoadPart(int start, int count, const QString &number, int status = 0, bool isBusy = false, bool isParent = false)
    // {
    //     return repo.LoadPart(start, count, typeDev, number, items, status, isBusy, isParent);
    // }

    size_t LoadPart2(int start, int count, const QString &number, QVector<int> vStatus, bool isBusy = false, bool isParent = false)
    {
        return repo.LoadPart(start, count, typeDev, number, items, vStatus, isBusy, isParent);
    }

protected:
    RepoMSSQL repo;

};


#endif // LISTDEVICE_H
