#ifndef USERS_H
#define USERS_H

#include <QList>
#include <QSet>
#include <infrastructure/general.h>


class Users
{
public:

    // static Users& getInstance()
    // {
    //     static Users instance;
    //     return instance;
    // }

    // Users() = default;
    // ~Users() = default;

    // Users(const Users&) = delete;
    // Users& operator=(const Users&) = delete;

    int id = 0;
    QString UserName;
    QString UserFullName;
    QString Pass;
    QSet<RolesType> listRoles;

    bool LoadRoles(int userId);
private:
};

#endif // USERS_H
