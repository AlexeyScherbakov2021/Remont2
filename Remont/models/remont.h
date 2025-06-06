#ifndef REMONT_H
#define REMONT_H

#include "Items.h"

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

    static void AddRemont(int idClaim, Items* item, int idPrevReason, QDateTime dateCreate);
};




#endif // REMONT_H
