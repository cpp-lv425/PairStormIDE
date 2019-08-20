#include "userslistmaker.h"

#include <QDir>

UsersListMaker::UsersListMaker()
{

}

QStringList UsersListMaker::usersListMake(const QString path)
{
    QDir directory(path);
    QStringList conFilesList = directory.entryList(QStringList() << "*.json", QDir::Files);

    // validate each file - not implemented
    foreach(QString filename, conFilesList)
    {
        // unctypt token
        // validation1: token.length() == 40

        // unctypt all fields using token as key
        // validation2: fileName without extention == userName

        // validation3: validate rest fields
        // if field corrupted use default value
    }
    return conFilesList;
}
