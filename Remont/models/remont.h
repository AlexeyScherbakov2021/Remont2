#ifndef REMONT_H
#define REMONT_H

#include <QDateTime>
#include <QString>


class Remont
{
public:
    int id = 0;
    int idItem = 0;
    int idClaim = 0;
    int idReason = 0;
    QString action;
    QString defect;
    QString remark;
    QDateTime startDate;
    QDateTime endDate;
};

#endif // REMONT_H
