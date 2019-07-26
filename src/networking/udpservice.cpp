#include "udpservice.h"

UdpService::UdpService(QObject *qObject) : QObject(qObject)
{
    // Configure standard UDP port number and empty datagram
    m_pendingDatagram = decltype(m_pendingDatagram) ();


    // Create & configure QUdpSocket
    m_udpSocketPtr = std::make_unique<QUdpSocket>(new QUdpSocket(this));
    m_udpSocketPtr->bind(QHostAddress::LocalHost, m_portNumber);

    // Save datagram on readyRead
    connect(
        m_udpSocketPtr.get(), &QUdpSocket::readyRead,
        this,                 &UdpService::saveDatagramOnReceival,
        Qt::UniqueConnection);
}

std::shared_ptr<UdpService> UdpService::getService()
{
    static std::shared_ptr<UdpService>
            serviceInstance(new UdpService);
    return serviceInstance;
}

void UdpService::broadcastDatagram(QString data)
{
    QByteArray datagram;
    datagram.append(data);
    // Broadcast datagram bytes
    m_udpSocketPtr->writeDatagram(
                datagram,
                QHostAddress::Broadcast,
                m_portNumber);
}

Datagram UdpService::getReceivedDatagram() const
{
    return m_pendingDatagram;
}

void UdpService::saveDatagramOnReceival()
{
    // Get size of the datagram
    qint64 size(m_udpSocketPtr->pendingDatagramSize());

    // Create & fill datagram attiributes
    QByteArray   data(static_cast<qint32>(size), ' ');
    QHostAddress ip;
    PortNumType  port;
    m_udpSocketPtr->readDatagram(
        data.data(), data.size(),
        &ip,
        &port);

    // Save datagram
    m_pendingDatagram.m_data = data;
    m_pendingDatagram.m_ip   = ip;
    m_pendingDatagram.m_port = port;

    emit newDatagramSaved();
}
