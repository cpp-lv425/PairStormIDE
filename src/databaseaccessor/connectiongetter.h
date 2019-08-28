#ifndef CONNECTIONGETTER_H
#define CONNECTIONGETTER_H
#include "connection.h"

class ConnectionGetter
{
    ConnectionGetter() = default;
public:
    static Connection* getDefaultConnection(const QString &path);
    static Connection* getDefaultConnection();
};

#endif // CONNECTIONGETTER_H
