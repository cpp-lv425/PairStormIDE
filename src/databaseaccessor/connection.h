#ifndef CONNECTION_H
#define CONNECTION_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QVariant>

class Connection
{
public:
    void openDatabase();
    ~Connection();
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
