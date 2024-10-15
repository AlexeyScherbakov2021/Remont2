#ifndef REMONT_H
#define REMONT_H

#include <QDateTime>
#include <QString>


class Remont
{
public:
    int id;
    int idParent;
    int idReclamation;
    int idReason;
    QString action;
    QString defect;
    QString remark;
    QDateTime startDate;
    QDateTime endDate;
};

#endif // REMONT_H
