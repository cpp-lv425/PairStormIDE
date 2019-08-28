/*
 * The purpose of this class is to provide list registered users according to provided path
*/
#ifndef USERSLISTMAKER_H
#define USERSLISTMAKER_H

class QStringList;
class QString;

class UsersListMaker
{
public:
    UsersListMaker();
    static QStringList usersListMake(const QString &path);
};

#endif // USERSLISTMAKER_H
