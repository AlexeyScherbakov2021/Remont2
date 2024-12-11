#ifndef REPOFP_H
#define REPOFP_H

#include <QSqlDatabase>

class Nakl
{
public:
    QString number;
    QString name;
    QString VNFT;
    quint32 count;
    QString plan;

    Nakl() : count(0) {}
};


class RepoFP
{
private:
    QSqlDatabase db;

public:
    RepoFP();
    // ~RepoFP();
    bool ConnectDb();

    // Nakl getDoc(const QString &number);
    bool getDoc(const QString &number, QList<Nakl> &listNakl);

};

#endif // REPOFP_H
