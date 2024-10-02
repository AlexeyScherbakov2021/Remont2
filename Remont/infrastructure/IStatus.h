#ifndef ISTATUS_H
#define ISTATUS_H

#include <QDateTime>
#include <QString>
#include <repomssql.h>

// class RepoMSSQL;

class Status
{
public:
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

    int id;
    int idDevice;
    Stat idStatus;
    QString nameStatus;
    QDateTime dateStatus;
    QString Comment;

    Status() : id(0), idDevice(0), idStatus(Status::NONE), dateStatus(QDateTime::currentDateTime()) {}

};

template <typename T>
class StatusList
{
public:
    QVector<Status> listStatus;


    void LoadStatus(T &device)
    {
        RepoMSSQL repo;
        repo.LoadStatus(device);
    }

    void AddStatus(T &device, Status::Stat idStat, QString comment = "")
    {
        AddStatus(device, idStat, QDateTime::currentDateTime(), comment );
    }


    void AddStatus(T &device, Status::Stat idStat, const QDateTime &dateRegister,  QString comment = "")
    {
        Status status;
        status.idStatus = idStat;
        status.idDevice = device.id;
        status.dateStatus = dateRegister;
        status.Comment = comment;
        RepoMSSQL repo;
        if(repo.AddStatus(device, status))
            listStatus.push_back(status);

    }


    QString getNameLastStatus() const
    {
        return listStatus.last().nameStatus;
    }

    QString getLastComment() const
    {
        return listStatus.last().Comment;
    }

};



#endif // ISTATUS_H
