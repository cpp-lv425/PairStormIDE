#include "userdb.h"
UserDb::UserDb(): Accessor()
{
}

void UserDb::addUserToDb(const User &user)
{
    execQuery(addUserQuery(user));
}

void UserDb::getUserFromDb(const int idUser)
{
    execQuery(getUserQuery(idUser));
}

void UserDb::getUserIdFromDb(const QString username)
{
    execQuery(getUserIdQuery(username));
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

QString UserDb::getUserIdQuery(const QString username)
{
    return "SELECT id FROM User WHERE nickname = '"
            + username + "'";
}
