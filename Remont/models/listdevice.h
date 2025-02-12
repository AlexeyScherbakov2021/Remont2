#ifndef LISTDEVICE_H
#define LISTDEVICE_H

#include "Items.h"
// #include <QDebug>
#include <QIcon>
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

    virtual bool AddItem(Items &item)
    {
        return repo.AddItem(item);
    }


    int GetRowFromId(int id)
    {
        for(int row = 0; row < items.size(); ++row)
        {
            if(items[row].id == id)
                return row;
        }

        return -1;
    }

    bool DeleteItemId(int id)
    {
        return repo.DeleteItem(id);
    }


    void DeleteItemFromList(int row)
    {
        items.removeAt(row);
    }

    virtual bool DeleteItem(int row)
    {
        bool res = false;
        Items item = items.at(row);
        if(item.id != 0)
        {
            res = repo.DeleteItem(item.id);
            if(res)
                items.removeAt(row);
        }
        return res;
    }

    virtual bool UpdateItem(Items &item)
    {
        return repo.UpdateItem(item);
    }

    void FindItems(const QString &number, int status = 0, bool isFree = false)
    {
        repo.FindItems(typeDev, number, items, status, isFree);
    }


    Items GetItem(int id)
    {
        Items resT;
        auto res = std::find_if(items.cbegin(), items.cend(), [id] (auto it) { return it.id == id; });
        if(res != items.cend())
            resT = *res;
        return resT;
    }

    Items* GetItem(QString number)
    {
        Items* resT = nullptr;
        auto res = std::find_if(items.begin(), items.end(), [number] (auto it) { return it.number == number; });
        if(res != items.cend())
            resT = (Items*)res;
        return resT;
    }


    virtual bool LoadChild(Items &item)
    {
        return repo.LoadChildItems(item.id, item.childItems);
    }

    // virtual void Load() = 0;

    virtual QVariant getData(int row, int col, int role) const
    {
        QVariant var;

        if(row > items.size())
            return var;

        auto item = items.at(row);

        if(role == Qt::DisplayRole)
        {
            switch(col)
            {
            case 1:
                var = item.number;
                break;
            case 2:
                var = item.VNFT;
                break;
            case 3:
                var = item.name;
                break;
            case 4:
                var = item.dateCreate.toString("dd.MM.yyyy");
                break;
            case 5:
                var = item.numberDoc;
                break;
            case 6:
                var = item.currStatus;
                break;
            }

        }
        else if(role == Qt::DecorationRole)
        {
            if(col == 0)
            {
                QString nameType;
                QString nameIcon;
                item.GetInfo(nameType, nameIcon);
                var = QIcon(nameIcon);
            }
        }
        return var;
    }


    virtual void setData(int /*row*/, int /*col*/, const QVariant /*value*/, int /*role*/) const
    {

    }



    virtual void GetHeader(QStringList& headers)
    {
        headers << "Тип" << "Номер" << "Обозначение" << "Наименование" << "Дата регистрации" << "Документ" << "Статус";
    }

    // size_t LoadPart(int start, int count, const QString &number, int status = 0, bool isBusy = false, bool isParent = false)
    // {
    //     return repo.LoadPart(start, count, typeDev, number, items, status, isBusy, isParent);
    // }

    int LoadPart2(int start, int count, const QString &number, QVector<int> vStatus, bool isBusy = false, bool isParent = false)
    {
        if(typeDev == ItemType::All)
            return repo.LoadPartAll(start, count, number, items, vStatus, isBusy, isParent);
        else
            return repo.LoadPart(start, count, typeDev, number, items, vStatus, isBusy, isParent);
    }

protected:
    RepoMSSQL repo;

};


#endif // LISTDEVICE_H
