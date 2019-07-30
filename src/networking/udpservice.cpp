#include "udpservice.h"
// ==========================================================================================
// ==========================================================================================
//                                                                    UDP SERVICE CONSTRUCTOR
UdpService::UdpService(QObject *qObject) :
    QObject(qObject), m_portNumber(g_defaultUdpPortNumber)
{
    configureSocket();

    // Save received datagrams on readyRead
    connect(
        m_udpSocketPtr.get(), &QUdpSocket::readyRead,
        this,                 &UdpService::saveDatagramOnReceival,
        Qt::UniqueConnection);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                               CONFIGURE SOCKET AND GET BROADCAST ADDRESSES
void UdpService::configureSocket()
{
    // Create & configure QUdpSocket
    m_udpSocketPtr = std::make_unique<QUdpSocket>(this);
    m_udpSocketPtr->bind(m_portNumber);

    // Establish broadcast addresses for each interface
    for (const auto & availableInt : QNetworkInterface::allInterfaces())
    {
        QNetworkInterface::InterfaceFlags intFlags = availableInt.flags();
        bool isInterfaceRunning(intFlags & QNetworkInterface::IsRunning);
        bool isInterfaceLoopBack(intFlags & QNetworkInterface::IsLoopBack);
        // Omit interfaces that are LoopBack or are not active
        if(!isInterfaceRunning || isInterfaceLoopBack) continue;

        // Add all broadcast IPv4 addresses from this interface
        for (const auto & ipEntry : availableInt.addressEntries())
        {
            if(ipEntry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                m_broadcastIps.push_back(ipEntry.broadcast());
            }
        }
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                   GET INSTANCE OF THE UDP SERVICE PROVIDER
std::shared_ptr<UdpService> UdpService::getService()
{
    static std::shared_ptr<UdpService>
            serviceInstance(new UdpService);

    return serviceInstance;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                               BROADCAST GIVEN DATAGRAM FROM ALL INTERFACES
void UdpService::broadcastDatagram(const QString & data)
{
    QByteArray datagram;
    datagram.append(data);

    // Broadcast through all broadcast Ip adresses
    for (const QHostAddress & broadcastIp : m_broadcastIps)
    {
        // Broadcast datagram bytes
        m_udpSocketPtr->writeDatagram(
                    datagram,
                    broadcastIp,
                    m_portNumber);
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                 GET LAST RECEIVED DATAGRAM
Datagram UdpService::getReceivedDatagram() const
{
    return m_pendingDatagram;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                               RECEIVE DATAGRAM AND SAVE IT
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
