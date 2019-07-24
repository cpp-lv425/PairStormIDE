#ifndef UPDSERVICE_H
#define UPDSERVICE_H

#include <memory>
#include <QObject>
#include <QUdpSocket>

// Udp service provider singleton
class UdpService : public QObject
{
    typedef quint16 PortNumberType;
    // Allow using signals and slots
    Q_OBJECT

    // Compose QUdpSocket
    std::unique_ptr<QUdpSocket> m_udpSocketPtr;

    // Port number
    PortNumberType m_udpPortNumber;

    // Received datagrams on the port m_udpPortNumber
    QString m_receivedBeaconDatagram;

    // Singleton means that default constructor is private
    explicit UdpService(QObject *qObject = nullptr);

public:

    // Also copying is restricted for singletons
    UdpService(UdpService const&) = delete;
    UdpService& operator=(UdpService const&) = delete;

    // Singleton instance generator
    static std::shared_ptr<UdpService> getInstance();

    // Beacon broadcaster
    void broadcastBeaconDatagram();

    QString getReceivedDatagram() const;

signals:
    void processReceivedDatagram();

public slots:
    // Receive beaconDatagram
    void receiveBeaconDatagram();
};

#endif // UPDSERVICE_H
