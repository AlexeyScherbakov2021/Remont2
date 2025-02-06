#include "IStatus.h"
#include <models/Items.h>

void StatusList::LoadStatus(Items& item)
{
    RepoMSSQL repo;
    repo.LoadStatus(item);
}

void StatusList::AddStatus(Items &item, Status::Stat idStat, const QString &comment)
{
    AddStatus(item, idStat, QDateTime::currentDateTime(), comment );
}

void StatusList::DeleteLastStatus(Items &item)
{
    // Q_UNUSED(item);
    RepoMSSQL repo;
    repo.DelLastStatus(item);
}


void StatusList::AddStatus(Items &item, Status::Stat idStat, const QDateTime &dateRegister,  const QString &comment)
{
    RepoMSSQL repo;
    Status status;
    status.idStatus = idStat;
    status.idItem = item.id;
    status.dateStatus = dateRegister;
    status.Comment = comment;
    status.nameStatus = repo.GetNameStatus((int)idStat);
}

QString StatusList::getNameLastStatus() const
{
    return listStatus.last().nameStatus;
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
