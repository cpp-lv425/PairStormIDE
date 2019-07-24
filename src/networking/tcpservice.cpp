#include "tcpservice.h"

TcpService::TcpService(QObject *qObject) : QObject(qObject)
{
    // Create object instance
}

std::shared_ptr<TcpService> TcpService::getInstance()
{
    static std::shared_ptr<TcpService> instance(new TcpService);
    return instance;
}
