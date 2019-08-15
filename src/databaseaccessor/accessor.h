#ifndef ACCESSOR_H
#define ACCESSOR_H
#include "connection.h"

class Accessor
{
public:
    Accessor();
    ~Accessor();
protected:
    void setQuery(QSqlDatabase db);
    void execQuery(QString query);
private:
    QSqlQuery guery;
    Connection* database;
};

#endif // ACCESSOR_H
