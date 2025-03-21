#ifndef ISTATUS_H
#define ISTATUS_H

#include <QDateTime>
#include <QString>
#include <repomssql.h>

class Items;

class Status
{
public:

    int id = 0;
    int idDevice = 0;
    int idItem = 0;
    StatusItem idStatus = StatusItem::NONE;
    QString nameStatus;
    QDateTime dateStatus = QDateTime::currentDateTime();
    int typeStatus = 0;
    QString Comment;
    int linkField = 0;
};


class StatusList
{
private:

public:

    QVector<Status> listStatus;
    void LoadStatus(Items& item);

    bool AddStatus(Items &item, StatusItem idStat, const QString &comment = "", int linkField = 0);
    bool AddStatus(Items &item, StatusItem idStat, const QDateTime &dateRegister,  const QString &comment = "", int linkField = 0);

    void DeleteLastStatus(Items &device, StatusItem status);
    QString getNameLastStatus() const;
    QString getLastComment() const;
    bool getIsRepair() const;

    void SetLastStatus(QString& currStatus, QString& commentStatus)
    {
        if(listStatus.size() > 0)
        {
            currStatus = listStatus.last().nameStatus;
            commentStatus = listStatus.last().Comment;
        }
    }

};


// template <typename T>
// class StatusList2
// {
// public:
//     QVector<Status> listStatus;

//     void LoadStatus(T &device)
//     {
//         RepoMSSQL repo;
//         repo.LoadStatus(device);
//     }

//     void AddStatus(T &device, Status::Stat idStat, const QString &comment = "")
//     {
//         AddStatus(device, idStat, QDateTime::currentDateTime(), comment );
//     }


//     void DeleteLastStatus(T &device)
//     {
//         RepoMSSQL repo;
//         repo.DelLastStatus(device);
//     }


//     void AddStatus(T &device, Status::Stat idStat, const QDateTime &dateRegister,  const QString &comment = "")
//     {
//         RepoMSSQL repo;
//         Status status;
//         status.idStatus = idStat;
//         status.idDevice = device.id;
//         status.dateStatus = dateRegister;
//         status.Comment = comment;
//         status.nameStatus = repo.GetNameStatus((int)idStat);
//         if(repo.AddStatus(device, status))
//         {
//             status.typeStatus = repo.GetTypeStatus(idStat);
//             listStatus.push_back(status);
//         }
//     }

//     QString getNameLastStatus() const
//     {
//         return listStatus.last().nameStatus;
//     }

//     QString getLastComment() const
//     {
//         return listStatus.size() > 0 ? listStatus.last().Comment : "";
//     }

//     bool getIsRepair() const
//     {
//         bool res = false;
//         if(listStatus.size() > 0)
//         {
//             res = listStatus.last().typeStatus == 1;

//             // res = (listStatus.last().idStatus <= Status::FAULTY_ON_OBJECT
//             //        || listStatus.last().idStatus >= Status::CORRECT_OSO);
//             // qDebug() << res;
//         //     return res;
//         }
//         // else
//         return res;
//     }

// };



#endif // ISTATUS_H
