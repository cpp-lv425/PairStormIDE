#include "userslistmaker.h"

#include <QDir>

UsersListMaker::UsersListMaker()
{

}

QStringList UsersListMaker::usersListMake(const QString &path)
{
    QDir directory(path);
    QStringList conFilesList = directory.entryList(QStringList() << "*.json", QDir::Files);

    // validate each file - not implemented yet
    foreach(QString filename, conFilesList)
    {   // to do
        // unctypt token
        // validation #1: token.length() == 40

        // unctypt all fields using token as key
        // validation #2: fileName without extention == userName

        // validation #3: validate rest fields
        //      if field corrupted use default value
    }
    return conFilesList;
}
