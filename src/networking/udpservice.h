#ifndef UPDSERVICE_H
#define UPDSERVICE_H

#include <memory>
#include <QObject>
#include <QUdpSocket>
#include "tcpudphelpers.h"

// SINGLETON
// UDP service provider
class UdpService : public QObject
{
    Q_OBJECT // for signals and slots


    // QUdpSocket composition
    std::unique_ptr<QUdpSocket> m_udpSocketPtr;


    // Standard port number for UDP communication
    // Received datagrams using the port m_portNumber
    const PortNumType m_portNumber = g_defaultUdpPortNumber;
    Datagram    m_pendingDatagram;

    explicit UdpService(QObject *qObject = nullptr);

public:
    UdpService(UdpService const&) = delete;
    UdpService& operator=(UdpService const&) = delete;

    // Service getter generator
    static std::shared_ptr<UdpService> getService();

    // Datagram broadcaster
    void broadcastDatagram(QString datagram);

    Datagram getReceivedDatagram() const;

signals:
    void newDatagramSaved();

public slots:
    void saveDatagramOnReceival();
};

#endif // UPDSERVICE_H
