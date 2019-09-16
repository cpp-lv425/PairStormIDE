#include "accessor.h"

Accessor::Accessor()
{
    database = ConnectionGetter::getDefaultConnection();
    database->openDatabase();
    setQuery(database->getDatabase());
}

void Accessor::setQuery(QSqlDatabase db)
{
    query = QSqlQuery(db);
}

void Accessor::execQuery(QString queryStr)
{
   query.prepare(queryStr);
}
