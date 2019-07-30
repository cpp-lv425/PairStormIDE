#include "apiparser.h"
// ==========================================================================================
// ==========================================================================================
//                                 CONSTRUCTOR FOR THE BASIC INTERFACE OF THE LOCAL CONNECTOR
LocalConnectorInterface::LocalConnectorInterface(QObject *qObject) : QObject(qObject) { }
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                   CREATE CONNECTOR & CONFIGURE UDP SERVICE
DefaultLocalConnector::DefaultLocalConnector() : LocalConnectorInterface()
{
    // Compose UDP service
    m_udpService = UdpService::getService();

    // Begin continiously discovering users
    connect(
        m_udpService.get(), &UdpService::newDatagramSaved,
        this,               &DefaultLocalConnector::addServerFromUdpDatagramOnReceive,
        Qt::UniqueConnection);

    // TODO connect configureServerOnLogin to the login signal
}
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
//                                                                            REQUEST SHARING
void DefaultLocalConnector::shareWithUser(QString userName)
{
    auto usersServerData =
            std::find_if(m_discoveredTcpServersAttrib.begin(),
                         m_discoveredTcpServersAttrib.end(),
                         [userName] (const ServerData & serverData)
                         {
                             return serverData.m_name == userName;
                         });
    // Return if not found any server with a given userName
    if (usersServerData == m_discoveredTcpServersAttrib.end()) return;
    // Else try to establish connection with server
    m_tcpService->connectToTcpServer(*usersServerData);
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
//                                                     LAUNCH SERVER AFTER USER HAS LOGGED IN
void DefaultLocalConnector::configureServerOnLogin(const QString & userName)
{
    // Create TCP server with name of the user
    m_tcpService = TcpService::getService(userName);

    // Generate processing when new user try to connect
    connect(
        m_tcpService.get(), &TcpService::socketConnected,
        this,               &DefaultLocalConnector::processConnectionOnRequest,
        Qt::UniqueConnection);

    // Save TCP segments on receival
    connect(
        m_tcpService.get(), &TcpService::newSegmentSaved,
        this,               &DefaultLocalConnector::processTcpSegmentOnReceive,
        Qt::UniqueConnection
    );

    // Broadcast server attributes each g_defaultBroadcastCycleMs milliseconds
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

    // Read received datagram and fill the information about discovered attributes
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
void DefaultLocalConnector::processConnectionOnRequest(std::shared_ptr<QTcpSocket> clientSocketPtr)
{
    // TODO process connection
    qDebug() << "sharing requested in connections manager";


    //emit sharingRequested(userName);
}




#ifdef CUSTOM_DEBUG

void DefaultLocalConnector::testConnectToLocalHost()
{
    qDebug() << "trying to connect to the local host";
    ServerData serverData;
    serverData.m_sourceIp = QHostAddress::LocalHost;
    serverData.m_port = g_defaultTcpPortNumber;
    m_tcpService->connectToTcpServer(serverData);
}
#endif //CUSTOM_DEBUG
