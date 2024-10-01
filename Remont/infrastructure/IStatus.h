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
    int idStatus;
    QString nameStatus;
    QDateTime dateStatus;
    QString Comment;

    Status() : id(0), dateStatus(QDateTime::currentDateTime()) {}

};

template <typename T>
class StatusList
{
public:
    QVector<Status> listStatus;

    void LoadStatus(T &device/*, RepoMSSQL *repo*/)
    {
        RepoMSSQL repo;
        repo.LoadStatus(device);
    }

    void AddStatus(T &device, Status stat/*, RepoMSSQL *repo*/)
    {


    }

    QString getNameLastStatus() const
    {
        return listStatus.last().nameStatus;
    }

};



#endif // ISTATUS_H
