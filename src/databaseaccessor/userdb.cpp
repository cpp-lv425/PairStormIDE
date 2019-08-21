#include "userdb.h"
UserDb::UserDb(): Accessor()
{
}

void UserDb::addUserToDb(const User &user)
{
    execQuery(addUserQuery(user));
    query.finish();
}

User UserDb::getUserFromDb(const int idUser)
{
    execQuery(getUserQuery(idUser));
    User rUser;
    fillStructUser(rUser);
    query.finish();
    return rUser;
}

QString UserDb::addUserQuery(const User& user)
{
    return "INSERT INTO User (nickname) VALUES ('"
            +user.mNickname +"')";
}

QString UserDb::getUserQuery(const int idUser)
{
    return "SELECT nickname FROM User WHERE id = "
            +QString::number(idUser);
}

void UserDb::fillStructUser(User &user)
{
    user.mNickname = query.record().value(0).toString();
}
