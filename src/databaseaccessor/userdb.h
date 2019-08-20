#ifndef USERDB_H
#define USERDB_H
#include "accessor.h"
#include "structsfordb.h"
class UserDb : public Accessor
{
public:
    UserDb();
    void addUserToDb(const User& user);
    void getUserFromDb(const int idUser);
    void getUserIdFromDb(const QString username);
private:
    QString addUserQuery(const User& user);
    QString getUserQuery(const int idUser);
    QString getUserIdQuery(const QString username);
};

#endif // USERDB_H
