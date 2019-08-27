#include "userdb.h"
UserDb::UserDb(): Accessor()
{
}

UserDb::~UserDb()
{
    query.finish();
}

void UserDb::addUserToDb(const User &user)
{
    execQuery(addUserQuery(user));
}

User UserDb::getUserFromDb(const int idUser)
{
    execQuery(getUserQuery(idUser));
    query.first();
    User rUser;
    fillStructUser(rUser);
    return rUser;
}

QVector<User> UserDb::getAllUsersFromDb()
{
    execQuery(numberOfUser());
    query.first();
    int count = query.record().value(0).toInt();
    QVector<User> rUser;
    for (int i = 0; i < count; i++)
    {
        rUser.push_back(getUserFromDb(i));
    }
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

QString UserDb::numberOfUser()
{
    return "Select count(user.id) from User";
}
