#include "accessor.h"

Accessor::Accessor()
{
    database= new Connection(Connection::getPath());
    setQuery(database->getDatabase());
}

Accessor::~Accessor()
{
    database->closeDb();
}

void Accessor::setQuery(QSqlDatabase db)
{
    query = QSqlQuery(db);
}

void Accessor::execQuery(QString queryStr)
{
    query.prepare(queryStr);
    query.exec();
}
