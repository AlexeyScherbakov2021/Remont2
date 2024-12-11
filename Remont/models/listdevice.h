#ifndef LISTDEVICE_H
#define LISTDEVICE_H

#include "entity.h"
#include <QList>
#include <repomssql.h>

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


    virtual bool DeleteItem(int id) = 0;

    virtual bool UpdateItem(T &item)
    {
        return repo.UpdateItem(item);
    }


    void FindItems(const QString &number, int status = 0, bool isFree = false)
    {
        repo.FindItems(number, listItems, status, isFree);
    }


    virtual T GetItem(int id)
    {
        T resT;
        auto res = std::find_if(listItems.cbegin(), listItems.cend(), [id] (auto it) { return it.id == id; });
        if(res != listItems.cend())
            resT = *res;
        return resT;
    }

    virtual T GetItem(QString number)
    {
        T resT;
        auto res = std::find_if(listItems.cbegin(), listItems.cend(), [number] (auto it) { return it.number == number; });
        if(res != listItems.cend())
            resT = *res;
        return resT;
    }


    virtual void LoadChild(T &item) = 0;


    virtual void Load() = 0;

    virtual QVariant getData(int row, int col) const = 0;
protected:
    RepoMSSQL repo;

};




#endif // LISTDEVICE_H
