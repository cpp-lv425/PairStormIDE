#include "connection.h"


Connection::Connection(QString path)
{
    mDb = QSqlDatabase::addDatabase(typeDatabase);
    mDb.setDatabaseName(path);
}

QSqlDatabase Connection::getDatabase()
{
    return mDb;
}

QString Connection::mPath = "";

void Connection::setpath(QString path)
{
    mPath = path;
}

QString Connection::getPath()
{
    return mPath;
}

void Connection::closeDb()
{
    mDb.close();
    mDb.removeDatabase(QSqlDatabase::defaultConnection);
}
