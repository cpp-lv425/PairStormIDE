#ifndef CONNECTIONSERVICEPROVIDER_H
#define CONNECTIONSERVICEPROVIDER_H

#include <QObject>
#include "localconnector.h"

class ConnectionServiceProvider : public QObject
{
    Q_OBJECT

    std::shared_ptr<DefaultLocalConnector> m_apiParser;

    std::string m_userName;

    explicit ConnectionServiceProvider(QObject *parent = nullptr);

public:

    // Also copying is restricted for singletons
    ConnectionServiceProvider(ConnectionServiceProvider const&) = delete;
    ConnectionServiceProvider& operator=(ConnectionServiceProvider const&) = delete;

    // Singleton instance generator
    static std::shared_ptr<ConnectionServiceProvider> getInstance();

signals:

public slots:
};

#endif // CONNECTIONSERVICEPROVIDER_H
