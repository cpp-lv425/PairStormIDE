#ifndef ACCESSOR_H
#define ACCESSOR_H
#include "connectiongetter.h"

class Accessor
{
public:
    Accessor();
    ~Accessor();
protected:
    void setQuery(QSqlDatabase db);
    void execQuery(QString query);
    QSqlQuery query;
private:
    Connection *database;
};

#endif // ACCESSOR_H
