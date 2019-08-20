#ifndef CONNECTIONGETTER_H
#define CONNECTIONGETTER_H
#include "connection.h"

class ConnectionGetter
{
    ConnectionGetter() = default;
public:
    static Connection* getDefaultConnection(QString path);
    static Connection* getDefaultConnection();

};

#endif // CONNECTIONGETTER_H
