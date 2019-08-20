#ifndef CONNECTION_H
#define CONNECTION_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

class Connection
{
public:

    ~Connection();
    void openDatabase();
    QSqlDatabase getDatabase();
    static QString getPath();
    void close();
private:
    Connection();
    static QString mPath;
    QSqlDatabase mDb;
    const QString typeDatabase = "QSQLITE";
    friend class ConnectionGetter;
};

#endif // CONNECTION_H
