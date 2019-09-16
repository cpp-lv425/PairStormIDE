#include "createdb.h"

CreateDB::CreateDB(): Accessor ()
{
}

CreateDB::~CreateDB()
{
    query.finish();
}

void CreateDB::addTableUser()
{
    execQuery(tableUserQuery());
}

void CreateDB::addTableMessage()
{
    execQuery(tableMessageQuery());
}

void CreateDB::addTableComment()
{
    execQuery(tableCommentQuery());
}

void CreateDB::addTableFile()
{
    execQuery(tableFileQuery());
}

QString CreateDB::tableUserQuery()
{
    return "CREATE TABLE IF NOT EXISTS User ("
            "id       INTEGER      PRIMARY KEY AUTOINCREMENT UNIQUE ON CONFLICT ROLLBACK, "
            "nickname VARCHAR (39) UNIQUE ON CONFLICT FAIL  NOT NULL )";
}

QString CreateDB::tableMessageQuery()
{
    return "CREATE TABLE IF NOT EXISTS Message ("
            "id          INTEGER  PRIMARY KEY AUTOINCREMENT, "
            "idUser      INTEGER  REFERENCES User (id) NOT NULL, "
            "messageText STRING, "
            "time        DATETIME DEFAULT (datetime('now')) )";
}

QString CreateDB::tableCommentQuery()
{
    return "CREATE TABLE IF NOT EXISTS Comment ("
        "line   INT, "
        "idFile          REFERENCES File (id) NOT NULL, "
        "idUser INTEGER  REFERENCES User (id) NOT NULL, "
        "text   TEXT, "
        "time   DATETIME DEFAULT (datetime('now') ), "
        "PRIMARY KEY (line, idFile))";
}

QString CreateDB::tableFileQuery()
{
    return "CREATE TABLE IF NOT EXISTS File ("
        "id   INTEGER      PRIMARY KEY AUTOINCREMENT, "
        "name VARCHAR (50) UNIQUE)";
}
