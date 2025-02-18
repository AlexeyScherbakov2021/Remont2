#ifndef LISTENTITY_H
#define LISTENTITY_H

#include "ItemsType.h"
#include "entity.h"
#include <repomssql.h>


template <class T>
class ListEntity
{

protected:
    QList<T> items;
    IndexType typeDev;

public:
    explicit ListEntity(IndexType _typeDev) : typeDev(_typeDev) {}
    ~ListEntity() { }

    virtual bool AddItem(T &item) = 0;
    virtual int GetRowFromId(int id) = 0;
    virtual bool DeleteItemId(int id) = 0;
    virtual void DeleteItemFromList(int row) = 0;
    virtual bool DeleteItem(int row) = 0;
    virtual bool UpdateItem(T &item) = 0;
    virtual T GetItem(int id) = 0;
    virtual T* GetItemAtRow(int row) = 0;
    virtual bool LoadChild(T &item) = 0;
    virtual QVariant getData(int row, int col, int role) const = 0;
    virtual void setData(int row, int col, const QVariant value, int role) const = 0;
    virtual void GetHeader(QStringList& headers) = 0;
    virtual int LoadPart2(int /*start*/, int /*count*/, const QString &/*number*/, QVector<int> /*vStatus*/, bool /*isBusy*/ = false, bool /*isParent*/ = false) {
        return 0;
    }

    // void FindItems(const QString &number, int status = 0, bool isFree = false)
    // {
    //     repo.FindItems(typeDev, number, items, status, isFree);
    // }


protected:
    RepoMSSQL repo;

};


#endif // LISTENTITY_H
