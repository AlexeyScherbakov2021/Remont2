#ifndef USERS_H
#define USERS_H

#include <QList>


class Users
{
public:

    static Users& getInstance()
    {
        static Users instance;
        return instance;
    }

    Users() = default;
    ~Users() = default;

    Users(const Users&) = delete;
    Users& operator=(const Users&) = delete;

    QString UserName;
    QString UserFullName;

    bool LoadRoles(int userId);

private:
    QList<int> listRoles;
};

#endif // USERS_H
