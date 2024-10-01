#ifndef REMONTSTEP_H
#define REMONTSTEP_H

#include <QDateTime>
#include <QString>


class RemontStep
{
public:
    int id;
    int RemontMId;
    int StatusId;
    QString Comment;
    QDateTime dateStep;

    RemontStep() : id{0} {}
};

#endif // REMONTSTEP_H
