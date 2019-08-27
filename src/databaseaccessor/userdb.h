#ifndef USERDB_H
#define USERDB_H
#include "accessor.h"
#include "structsfordb.h"
class UserDb : public Accessor
{
public:
    UserDb();
    ~UserDb();
    void addUserToDb(const User& user);
    User getUserFromDb(const int idUser);
    QVector<User> getAllUsersFromDb();
private:
    QString addUserQuery(const User& user);
    QString getUserQuery(const int idUser);
    void fillStructUser(User &user);
    QString numberOfUser();
};

#endif // USERDB_H
