#include "connection.h"
#include <QDebug>

Connection::Connection()
{
     mDb = QSqlDatabase::addDatabase(typeDatabase);
     qDebug()<<"connection";
}

Connection::~Connection()
{
    mDb.close();
    qDebug()<<"closed";
}

void Connection::openDatabase()
{
    mDb = QSqlDatabase::database();
    qDebug()<<mPath;
    mDb.setDatabaseName(mPath);
    if (mDb.open())
        qDebug()<<"opened";
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

