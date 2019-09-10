#include "connectiongetter.h"

Connection* ConnectionGetter::getDefaultConnection(const QString &path)
{
    Connection::mPath = path;
    return getDefaultConnection();
}

Connection *ConnectionGetter::getDefaultConnection()
{
    static Connection *defaultConn = new Connection();
    return defaultConn;
}
