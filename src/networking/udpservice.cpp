#include "udpservice.h"

UdpService::UdpService(QObject *qObject) : QObject(qObject)
{
    // Create udpSocket instance
    m_udpSocketPtr = std::make_unique<QUdpSocket>(new QUdpSocket(this));

    // Configure port number for udp transmission
    m_udpPortNumber = static_cast<PortNumberType>(32807);

    // Create empty pending datagram
    m_receivedBeaconDatagram = decltype(m_receivedBeaconDatagram)();

    // Configure udpSocket instance
    m_udpSocketPtr->bind(QHostAddress::LocalHost, m_udpPortNumber);

    // Uniquely connect readyRead signal to the processBeacondDatagram slot
    connect(
        m_udpSocketPtr.get(),   &QUdpSocket::readyRead,
        this,                   &UdpService::receiveBeaconDatagram,
        Qt::UniqueConnection
    );
}

std::shared_ptr<UdpService> UdpService::getInstance()
{
    static std::shared_ptr<UdpService> instance(new UdpService);
    return instance;
}

void UdpService::broadcastBeaconDatagram()
{
    QByteArray beaconDatagram;
    beaconDatagram.append("Hello from PairStorm application");

    m_udpSocketPtr->writeDatagram(beaconDatagram, QHostAddress::Broadcast, m_udpPortNumber);
    return;
}

QString UdpService::getReceivedDatagram() const
{
    return m_receivedBeaconDatagram;
}

void UdpService::receiveBeaconDatagram()
{
    // Create beaconDatagram of appropriate size
    QByteArray beaconDatagram;
    beaconDatagram.resize(
        static_cast<int>(m_udpSocketPtr->pendingDatagramSize())
    );

    // Create temporar
    QHostAddress hostIP;
    PortNumberType hostPortNumber;

    m_udpSocketPtr->readDatagram(
        beaconDatagram.data(), beaconDatagram.size(),
        &hostIP,
        &hostPortNumber
    );

    // Embed host ip into the datagram
    beaconDatagram.append("\nIP(" + hostIP.toString() + ")");

    // Push datagram to the received datagrams
    m_receivedBeaconDatagram = beaconDatagram;
    emit processReceivedDatagram();
}
