#ifndef ISTATUS_H
#define ISTATUS_H

#include <QDateTime>
#include <QString>
#include <repomssql.h>

class Items;

class Status
{
public:
    enum Stat {
        NONE,               // нет статуса
        CREATE,             // создан
        FAULTY,             // неисправен на производстве
        CORRECT,            // исправен на производстве
        INSTALL,            // установен в оборудовании
        SHIPPED,            // отгружен
        WORK,               // в работе
        FAULTY_ON_OBJECT,   // неисправен на объекте
        FAULTY_ON_OSO,      // неисправен в ОСО
        REMONT,             // в ремонте
        CORRECT_OSO,        // исправен б/у в осо
        END_WORK            // утилизирован
    };

    int id = 0;
    int idDevice = 0;
    int idItem = 0;
    Stat idStatus = Status::NONE;
    QString nameStatus;
    QDateTime dateStatus = QDateTime::currentDateTime();
    int typeStatus = 0;
    QString Comment;

};


class StatusList
{
private:

public:

    QVector<Status> listStatus;
    void LoadStatus(Items& item);
    bool AddStatus(Items &item, Status::Stat idStat, const QString &comment = "");
    void DeleteLastStatus(Items &device);
    bool AddStatus(Items &item, Status::Stat idStat, const QDateTime &dateRegister,  const QString &comment = "");
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
