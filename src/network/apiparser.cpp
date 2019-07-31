#include "apiparser.h"
// ==========================================================================================
// ==========================================================================================
//                                 CONSTRUCTOR FOR THE BASIC INTERFACE OF THE LOCAL CONNECTOR
LocalConnectorInterface::LocalConnectorInterface(QObject *qObject) : QObject(qObject) { }
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                           CREATE CONNECTOR
DefaultLocalConnector::DefaultLocalConnector() : LocalConnectorInterface() { }
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                     GET CONNECTOR INSTANCE
std::shared_ptr<DefaultLocalConnector> DefaultLocalConnector::generateConnector()
{
    static std::shared_ptr<DefaultLocalConnector>
            instance(new DefaultLocalConnector);

    return instance;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                   LAUNCH SERVICES AFTER USER HAS LOGGED IN
void DefaultLocalConnector::configureServiceOnLogin(const QString & userName)
{
    // Return if services has previously been configured
    if (m_udpService && m_tcpService)
    {
        return;
    }
    // Configure the UDP service & TCP service
    m_udpService = UdpService::getService();
    m_tcpService = TcpService::getService(userName);
    if (!m_tcpService->isActive())
    {
        emit serviceFailed();
        return;
    }

    // Begin discovering neighbor servers using UDP service
    connect(
        m_udpService.get(), &UdpService::newDatagramSaved,
        this,               &DefaultLocalConnector::addServerFromUdpDatagramOnReceive,
        Qt::UniqueConnection);

    // Begin capturing incomming user connections using TCP service
    connect(
        m_tcpService.get(), &TcpService::socketConnected,
        this,               &DefaultLocalConnector::processConnectionOnRequest,
        Qt::UniqueConnection);

    // Begin saving incoming messages using TCP service
    connect(
        m_tcpService.get(), &TcpService::newSegmentSaved,
        this,               &DefaultLocalConnector::processTcpSegmentOnReceive,
        Qt::UniqueConnection
    );


    // Broadcast server attributes each g_defaultBroadcastCycleMs
    // milliseconds using UDP service
    m_internalBroadcastTimer = std::make_unique<QTimer>();
    connect(
        m_internalBroadcastTimer.get(), &QTimer::timeout,
        this,                           &DefaultLocalConnector::broadcastServerAttributesOnTimerTick,
        Qt::UniqueConnection);
    m_internalBroadcastTimer->start(g_defaultBroadcastCycleMs);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                          ADD USER FROM DISCOVERED DATAGRAM
void DefaultLocalConnector::addServerFromUdpDatagramOnReceive()
{
    ServerData discoveredServerAttributes;
    // Read received datagram and fill it with attributes of the discovered server
    Datagram   datagramContent = m_udpService->getReceivedDatagram();
    discoveredServerAttributes.fromJsonQString(datagramContent.m_data);
    discoveredServerAttributes.m_sourceIp = datagramContent.m_ip;


    if (discoveredServerAttributes.empty())
    {
        // Datagram was corrupted
        return;
    }
    if (discoveredServerAttributes.m_name ==
        m_tcpService->getServerAttributes().m_name)
    {
        // Discovered server name matches current server name
        return;
    }
    auto attribPtr =
            std::find_if(m_discoveredTcpServersAttrib.cbegin(),
                         m_discoveredTcpServersAttrib.cend(),
                         [discoveredServerAttributes] (const ServerData & inServerData)
                         {
                             return inServerData.m_name ==
                                    discoveredServerAttributes.m_name;
                         });
    if (attribPtr != m_discoveredTcpServersAttrib.cend())
    {
        // Server with a given name has already been discovered
        return;
    }
    // Finally, save information about server & emit "user discovered" signal
    m_discoveredTcpServersAttrib.push_back(discoveredServerAttributes);

    emit newUserDiscovered();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                       GET DISCOVERED USERS
QVector<QString> DefaultLocalConnector::getOnlineUsers() const
{
    QVector<QString> userNames;
    // Get user names from discovered servers
    for (const auto & serverAttrib : m_discoveredTcpServersAttrib)
        userNames.push_back(serverAttrib.m_name);

    return userNames;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                        GET USERS, ALLOWED FOR SHARING WITH
QVector<QString> DefaultLocalConnector::getConnectedUsers() const
{
    QVector<QString> userNames;
    // Get user names from connected servers
    for (const auto & serverAttrib : m_connectedTcpServersAttrib)
        userNames.push_back(serverAttrib.m_name);

    return userNames;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                            REQUEST SHARING
void DefaultLocalConnector::requestSharingWithUser(QString userName)
{
    auto serverAttributesPtr =
            std::find_if(m_discoveredTcpServersAttrib.begin(),
                         m_discoveredTcpServersAttrib.end(),
                         [userName] (const ServerData & serverData)
                         {
                             return serverData.m_name == userName;
                         });
    // If found user with a given userName
    if (serverAttributesPtr != m_discoveredTcpServersAttrib.end())
    {
        m_connectedTcpServersAttrib.push_back(*serverAttributesPtr);
        //m_tcpService->connectToTcpServer(*serverAttributes);
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                  BROADCAST DATAGRAM WITH SERVER ATTRIBUTES
void DefaultLocalConnector::broadcastServerAttributesOnTimerTick()
{
    // Push attributes to the Json bearer string & broadcast it
    ServerData launchedTcpServerAttrib = m_tcpService->getServerAttributes();
    QString serverData(launchedTcpServerAttrib.toJsonQString());
    m_udpService->broadcastDatagram(serverData);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                             TRANSLATE RECEIVED TCP SEGMENT
void DefaultLocalConnector::processTcpSegmentOnReceive()
{
    Segment data = m_tcpService->getReceivedSegment();


    // TODO processing




#ifdef CUSTOM_DEBUG
    qDebug() << "____________________________________________________________";
    qDebug() << "TCP SEGMENT HAS BEEN RECEIVED: ";
    qDebug() << "data ->" << data.m_data;
    qDebug() << "ip -> " << data.m_ip.toString();
    qDebug() << "port ->" << data.m_port;
    qDebug() << "____________________________________________________________";
#endif // CUSTOM_DEBUG
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                         REMOVE USERNAME TO SOCKET RELATION
void DefaultLocalConnector::processConnectionOnRequest(QTcpSocket * clientSocketPtr)
{
    // TODO process connection
    qDebug() << "sharing requested in connections manager";


    //emit sharingRequested(userName);
}




#ifdef CUSTOM_DEBUG

void DefaultLocalConnector::testConnectToValik()
{
    qDebug() << "trying to connect to other server";
    ServerData serverData;
    serverData.m_sourceIp = QHostAddress("192.168.103.19");
    serverData.m_port = 37108;
    if(m_tcpService->connectToTcpServer(serverData))
        qDebug() << "successful connection";
    else {
        qDebug() << "connection failed";
    }
}
#endif //CUSTOM_DEBUG
