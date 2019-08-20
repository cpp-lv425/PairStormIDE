#include "accessor.h"
#include <QDebug>

Accessor::Accessor()
{
    database = ConnectionGetter::getDefaultConnection();
    database->openDatabase();
    setQuery(database->getDatabase());
}

Accessor::~Accessor()
{
    query.clear();
    qDebug()<<"clear query";
}

void Accessor::setQuery(QSqlDatabase db)
{
    query = QSqlQuery(db);
}

void Accessor::execQuery(QString queryStr)
{
   query.prepare(queryStr);
   if(query.exec())
   {
       qDebug()<<"executed query";
   }
   else
   {
        qDebug()<<"not executed query";
   }
}
