#include "udpservice.h"
// ==========================================================================================
// ==========================================================================================
//                                                                    UDP SERVICE CONSTRUCTOR
UdpService::UdpService(QObject *qObject) :
    QObject(qObject), mcSocketPortNumber(gDefaultUdpPortNumber)
{
    configureSocket();

    // Save received datagrams on readyRead
    connect(
        mpUdpSocket.get(), &QUdpSocket::readyRead,
        this,                 &UdpService::saveDatagramOnReceival,
        Qt::UniqueConnection);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                              CONFIGURE SOCKET AND SAVE BROADCAST ADDRESSES
void UdpService::configureSocket()
{
    // Create & configure QUdpSocket
    mpUdpSocket = std::make_unique<QUdpSocket>(this);
    mpUdpSocket->bind(mcSocketPortNumber);

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
                mBroadcastAddresses.push_back(ipEntry.broadcast());
            }
        }
    }
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
    for (const QHostAddress & broadcastIp : mBroadcastAddresses)
    {
        // Broadcast datagram bytes
        mpUdpSocket->writeDatagram(datagram, broadcastIp, mcSocketPortNumber);
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                 GET LAST RECEIVED DATAGRAM
Datagram UdpService::getReceivedDatagram() const
{
    return mPendingDatagram;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                  SAVE DATAGRAM ON RECEIVAL
void UdpService::saveDatagramOnReceival()
{
    // Get size of the datagram
    SizeType size(mpUdpSocket->pendingDatagramSize());

    // Create & fill datagram attiributes
    QByteArray   data(static_cast<qint32>(size), ' ');
    QHostAddress ip;
    PortNumType  port;
    mpUdpSocket->readDatagram(data.data(), data.size(), &ip, &port);

    // Save datagram
    mPendingDatagram.mContent = data;
    mPendingDatagram.mIp   = ip;
    mPendingDatagram.mPort = port;

    emit newDatagramSaved();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                     UDP SERVICE DESTRUCTOR
UdpService::~UdpService()
{
    mpUdpSocket->close();
}
