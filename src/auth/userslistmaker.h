#ifndef USERSLISTMAKER_H
#define USERSLISTMAKER_H

//QT_BEGIN_NAMESPACE
class QStringList;
class QString;
//QT_END_NAMESPACE

class UsersListMaker
{
public:
    UsersListMaker();
    static QStringList usersListMake(const QString path);
};

#endif // USERSLISTMAKER_H
