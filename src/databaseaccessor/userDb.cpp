#include "userDb.h"
UserDb::UserDb(): Accessor()
{
}

void UserDb::addUserToDb(const dbUser &user)
{
    execQuery(addUserQuery(user));
}

void UserDb::getUserFromDb(const int idUser)
{
    execQuery(getUserQuery(idUser));
}

QString UserDb::addUserQuery(const dbUser& user)
{
    return "INSERT INTO User (nickname) VALUES ('"
            +user.mNickname +"')";
}

QString UserDb::getUserQuery(const int idUser)
{
    return "SELECT nickname FROM User WHERE id = "
            +QString::number(idUser);
}
