#include "IStatus.h"
#include <models/Items.h>

void StatusList::LoadStatus(Items& item)
{
    RepoMSSQL repo;
    repo.LoadStatus(item);
}

bool StatusList::AddStatus(Items &item, StatusItem idStat, const QString &comment)
{
    return AddStatus(item, idStat, QDateTime::currentDateTime(), comment );
}

void StatusList::DeleteLastStatus(Items &item)
{
    // Q_UNUSED(item);
    RepoMSSQL repo;
    if(repo.DelLastStatus(item))
        item.listStatus.removeLast();
}


bool StatusList::AddStatus(Items &item, StatusItem idStat, const QDateTime &dateRegister,  const QString &comment)
{
    bool res;
    RepoMSSQL repo;
    Status status;
    status.idStatus = idStat;
    status.idItem = item.id;
    status.dateStatus = dateRegister;
    status.Comment = comment;
    status.nameStatus = repo.GetNameStatus((int)idStat);
    if(res = repo.AddStatus(item, status))
    {
        item.listStatus.push_back(status);
        item.currStatus = status.nameStatus;
    }

    return res;
}

QString StatusList::getNameLastStatus() const
{
    QString stat;
    if (listStatus.size() > 0)
        stat = listStatus.last().nameStatus;
    return stat;
}

QString StatusList::getLastComment() const
{
    return listStatus.size() > 0 ? listStatus.last().Comment : "";
}

bool StatusList::getIsRepair() const
{
    bool res = false;
    if(listStatus.size() > 0)
    {
        res = listStatus.last().typeStatus == 1;
    }
    return res;
}
