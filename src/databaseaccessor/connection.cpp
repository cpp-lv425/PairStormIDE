#include "connection.h"
#include <QDebug>

Connection::Connection()
{
     mDb = QSqlDatabase::addDatabase(typeDatabase);
}

Connection::~Connection()
{
    mDb.removeDatabase(QLatin1String(QSqlDatabase::defaultConnection));
}

void Connection::openDatabase()
{
    mDb = QSqlDatabase::database();
    mDb.setDatabaseName(mPath);
    mDb.open();
}

QSqlDatabase Connection::getDatabase()
{
    return mDb;
}

QString Connection::mPath = "";

QString Connection::getPath()
{
    return mPath;
}

void Connection::close()
{
    mDb.close();
}

