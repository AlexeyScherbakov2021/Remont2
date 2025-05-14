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
    QString numberDoc;
};


class StatusList
{
private:

public:
    QVector<Status> listStatus;
    void LoadStatus(Items& item);

    bool AddStatus(Items &item, StatusItem idStat, const QString &comment = "", int linkField = 0);
    bool AddStatus(Items &item, StatusItem idStat, const QDateTime &dateRegister,  const QString &comment = "", int linkField = 0);
    bool AddStatus(Items &item, Status& status);

    void DeleteLastStatus(Items &device, StatusItem status);
    QString getNameLastStatus() const;
    QString getLastComment() const;
    bool getIsRepair() const;
    // void SetLastStatus(QString& currStatus, QString& commentStatus) const;
    // bool TestRemontStatus();

};


#endif // ISTATUS_H
