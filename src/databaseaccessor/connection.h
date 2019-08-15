#ifndef CONNECTION_H
#define CONNECTION_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class Connection
{
public:
    Connection(QString path);
    QSqlDatabase getDatabase();
    void setpath(QString path);
    static QString getPath();
    void closeDb();
private:
    static QString mPath;
    QSqlDatabase mDb;
    const QString typeDatabase = "QSQLITE";
};

#endif // CONNECTION_H
