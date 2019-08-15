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
    guery = QSqlQuery(db);
}

void Accessor::execQuery(QString queryStr)
{
    guery.prepare(queryStr);
    guery.exec();
}
