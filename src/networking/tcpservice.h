#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <memory>
#include <QObject>
#include <QTcpServer>

// Udp service provider singleton
class TcpService : public QObject
{
    // Allow using signals and slots
    Q_OBJECT

    // Singleton means that default constructor is private
    explicit TcpService(QObject *qObject = nullptr);

public:

    // Also copying is restricted for singletons
    TcpService(TcpService const&) = delete;
    TcpService& operator=(TcpService const&) = delete;

    // Singleton instance generator
    static std::shared_ptr<TcpService> getInstance();

private:

};

#endif // TCPSERVICE_H
