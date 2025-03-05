#include "users.h"

#include <repomssql.h>


bool Users::LoadRoles(int userId)
{
    RepoMSSQL repo;
    repo.LoadRolesUser(userId, listRoles);
    return true;
}
