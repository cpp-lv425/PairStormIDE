#ifndef USERDB_H
#define USERDB_H
#include "accessor.h"
#include "structsfordb.h"
class UserDb : public Accessor
{
public:
    UserDb();
    void addUserToDb(const User& user);
    User getUserFromDb(const int idUser);
private:
    QString addUserQuery(const User& user);
    QString getUserQuery(const int idUser);
    void fillStructUser(User &user);
};

#endif // USERDB_H
