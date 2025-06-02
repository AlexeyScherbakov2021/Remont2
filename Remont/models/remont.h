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
    int idPrevReason = 0;
    QString action;
    QString defect;
    QString remark;
    QDateTime regDate;
    QDateTime startDate;
    QDateTime endDate;
    QString nameReason;
    QString namePrevReason;
};

#endif // REMONT_H
