#ifndef ITEMS_H
#define ITEMS_H

// #include "ItemsType.h"
#include "entity.h"
#include <QDateTime>
#include <QString>
#include <infrastructure/IStatus.h>

class Items : public IEntity, public StatusList
{
public:
    int idSet = 0;
    QString number2;
    QDateTime dateOn;
    QDateTime dateOff;
    int garantMonth = 0;
    QDateTime dateGarant;
    bool isZip = false;

    ItemType type;

    QString VNFT;
    QString currStatus;
    QString commentStatus;

    QString GetDefaultName()
    {
        return number + " (" + type.typeName + " " + type.VNFT + ")";
    }

    QString FullNameAndComment()
    {
        QString s = getLastComment();
        if(!s.isEmpty())
            s = "(" + s + ")";
        return name + " №" + number + s;
    }

    IndexType GetInfo(QString &nameType, QString &nameIcon) const override
    {
        switch(type.indexType)
        {
        case IndexType::Product:
            nameType = "Изделие";
            nameIcon = ":/image/product.png";
            break;
        case IndexType::Modul:
            nameType = "Модуль";
            nameIcon = ":/image/modul.png";
            break;
        case IndexType::Plate:
            nameType = "Плата";
            nameIcon = ":/image/network_adapter.png";
            break;
        case IndexType::All:
            nameType = "Любой";
            break;
        case IndexType::ClaimType:
        default:
            break;
        }

        return type.indexType;
    }


    void SetLastStatus(QString& currStatus, QString& commentStatus) const
    {
        if(listStatus.size() > 0)
        {
            currStatus = listStatus.last().nameStatus;
            commentStatus = listStatus.last().Comment;
        }
    }


    bool TestChildGoodStatus()
    {
        bool res = true;
        if(childItems.size() == 0)
        {
            RepoMSSQL repo;
            repo.LoadChildItems(id, childItems);
        }

        for(auto &it : childItems)
        {
            if(it.listStatus.size() > 0 && it.listStatus.last().typeStatus == TypeStatus::REMONT_STATUS)
            {
                res = false;
                break;
            }
        }
        return res;
    }

    bool TestChildStatus(StatusItem status)
    {
        bool res = true;
        if(childItems.size() == 0)
        {
            RepoMSSQL repo;
            repo.LoadChildItems(id, childItems);
        }

        for(auto &it : childItems)
        {
            if(it.listStatus.size() > 0 && it.listStatus.last().idStatus == status)
            {
                res = false;
                break;
            }
        }
        return res;
    }

    // bool operator==(const Items &other) const { return this->id == other.id; }

    // inline size_t qHash(const Items &key/*, uint seed*/){
    //     return qHash(key.id);
    // StatusList listStatus;
};


#endif // ITEMS_H
