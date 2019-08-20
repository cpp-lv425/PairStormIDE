#ifndef USERDB_H
#define USERDB_H
#include "accessor.h"
#include "structsfordb.h"
class UserDb : public Accessor
{
public:
    UserDb();
    void addUserToDb(const dbUser& user);
    void getUserFromDb(const int idUser);
private:
    QString addUserQuery(const dbUser& user);
    QString getUserQuery(const int idUser);
};

#endif // USERDB_H
