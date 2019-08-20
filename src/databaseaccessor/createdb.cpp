#include "createdb.h"
#include <QDebug>

createDB::createDB() :Accessor ()
{
}

void createDB::addTableUser()
{
    execQuery(tableUserQuery());
}

void createDB::addTableMessage()
{
    execQuery(tableMessageQuery());
}

void createDB::addTableComment()
{
    execQuery(tableCommentQuery());
}

void createDB::addTableFile()
{
    execQuery(tableFileQuery());
}

QString createDB::tableUserQuery()
{
    return "CREATE TABLE IF NOT EXISTS User ("
            "id       INTEGER      PRIMARY KEY AUTOINCREMENT UNIQUE ON CONFLICT ROLLBACK, "
            "nickname VARCHAR (39) UNIQUE ON CONFLICT FAIL  NOT NULL )";
}

QString createDB::tableMessageQuery()
{
    return "CREATE TABLE IF NOT EXISTS Message ("
            "ID          INTEGER  PRIMARY KEY AUTOINCREMENT, "
            "idUser      INTEGER  REFERENCES User (id) NOT NULL, "
            "messageText STRING, "
            "time        DATETIME DEFAULT (datetime('now')) )";
}

QString createDB::tableCommentQuery()
{
    return "CREATE TABLE IF NOT EXISTS Comment ("
        "line   INT, "
        "idFile          REFERENCES File (ID), "
        "idUser INTEGER  REFERENCES User (id), "
        "text   TEXT, "
        "time   DATETIME DEFAULT (datetime('now') ), "
        "PRIMARY KEY (line, idFile))";
}

QString createDB::tableFileQuery()
{
    return "CREATE TABLE IF NOT EXISTS File ("
        "ID   INTEGER      PRIMARY KEY AUTOINCREMENT, "
        "name VARCHAR (50) UNIQUE)";
}
