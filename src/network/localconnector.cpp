#include "localconnector.h"
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

    startBroadcastingAttributes();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                       GET DISCOVERED USERS
QStringList DefaultLocalConnector::getOnlineUsers() const
{
    QStringList userNames;
    // Get user names from discovered servers
    for (const auto & serverAttrib : m_discoveredTcpServersAttrib)
        userNames.push_back(serverAttrib.m_name);

    return userNames;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                        GET USERS, ALLOWED FOR SHARING WITH
QStringList DefaultLocalConnector::getConnectedUsers() const
{
    QStringList userNames;
    // Get user names from connected servers
    for (const auto & serverAttrib : m_connectedTcpServersAttrib)
        userNames.push_back(serverAttrib.m_name);

    return userNames;
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

    emit newUserDiscovered(this);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                            REQUEST SHARING
void DefaultLocalConnector::startSharing(const QString & userName)
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
        Message message;
        message.m_sourceName = m_tcpService->getServerAttributes().m_name;
        message.m_data       = QString();
        message.m_type       = MessageType::MessageTypeRequestSharingMessage;
        if(m_tcpService->sendDataToTcpServer(message.toJsonQString(), *serverAttributesPtr))
        {
            qDebug() << "sharing was started";
            m_connectedTcpServersAttrib.push_back(*serverAttributesPtr);
            emit newUserConnected(this);
        }
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                       START BROADCASTING SERVER ATTRIBUTES
void DefaultLocalConnector::startBroadcastingAttributes()
{
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
    Segment segment = m_tcpService->getReceivedSegment();
    Message message;
    message.fromJsonQString(segment.m_data);

    if (message.empty())
    {
        return;
    }

    switch(message.m_type)
    {
    case MessageType::MessageTypeRequestSharingMessage:
        qDebug() << message.m_sourceName << " request sharing changes";
        emit sharingRequested(message.m_sourceName);
        break;
    case MessageType::MessageTypeChatMessage:
        break;
    case MessageType::MessageTypeChangesMessage:
        break;
    }

#ifdef CUSTOM_DEBUG
    qDebug() << "____________________________________________________________";
    qDebug() << "TCP SEGMENT HAS BEEN RECEIVED: ";
    qDebug() << "data ->" << segment.m_data;
    qDebug() << "ip -> "  << segment.m_ip.toString();
    qDebug() << "port ->" << segment.m_port;
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

void DefaultLocalConnector::testSendHelloToLastServer()
{
    ServerData serverData;
    if(!m_discoveredTcpServersAttrib.empty())
    {
        serverData = m_discoveredTcpServersAttrib.back();
    }

    qDebug() << "try to send hello to server " << serverData.m_name;

    QString data("hello from ");
    data.append(m_tcpService->getServerAttributes().m_name);
    data.append(" to ");
    data.append(serverData.m_name);

    Message message;
    message.m_data = data;
    message.m_type = MessageType::MessageTypeChatMessage;
    message.m_sourceName = m_tcpService->getServerAttributes().m_name;

    if(m_tcpService->sendDataToTcpServer(data, serverData))
    {
        qDebug() << "message is sent";
    }
    else
    {
        qDebug() << "message is not sent";
    }
    //startSharing(serverData.m_name);
}
#endif //CUSTOM_DEBUG
