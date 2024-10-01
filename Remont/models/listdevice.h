#ifndef LISTDEVICE_H
#define LISTDEVICE_H

#include "entity.h"
// #include "product.h"

#include <QList>
#include <repomssql.h>
// #include "setterout.h"
// #include "shipment.h"

template <typename T>
class ListDevice
{
public:
    QList<T> listItems;

    explicit ListDevice() {}


    virtual bool AddItem(T &item)
    {
        return repo.AddItem(item);
    }


    virtual bool DeleteItem(T &item)
    {
        return repo.DeleteItem(item);
    }


    virtual bool UpdateItem(T &item)
    {
        return repo.UpdateItem(item);
    }


    void FindItems(const QString &number, int status = 0)
    {
        repo.FindItems(number, listItems, status);
    }


    void FindItems(int status = 0)
    {
        repo.FindItems(listItems, status);
    }

    virtual T GetItem(int id) = 0;
    // {
    //     T res = repo.GetItem(id);
    //     return res;
    // }


    virtual void LoadChild(T &item) = 0;


    virtual void Load() = 0;
    virtual QVariant getData(int row, int col) const = 0;
protected:
    RepoMSSQL repo;

};




#endif // LISTDEVICE_H
