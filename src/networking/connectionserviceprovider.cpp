#include "connectionserviceprovider.h"

ConnectionServiceProvider::ConnectionServiceProvider(QObject *parent) : QObject(parent)
{
    m_apiParser = ApiParser::getInstance();

}

std::shared_ptr<ConnectionServiceProvider> ConnectionServiceProvider::getInstance()
{
    static std::shared_ptr<ConnectionServiceProvider> instance(new ConnectionServiceProvider);
    return instance;
}
