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
    m_udpService = std::unique_ptr<UdpService>(new UdpService());
    m_tcpService = std::unique_ptr<TcpService>(new TcpService(userName));
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

    // Begin saving incoming messages using TCP service
    connect(
        m_tcpService.get(), &TcpService::newSegmentSaved,
        this,               &DefaultLocalConnector::processTcpSegmentOnReceive,
        Qt::UniqueConnection
    );

    startBroadcastingAttributes();
    startClearingOutdatedAttributes();
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
        this,                           &DefaultLocalConnector::shareAttributesOnTimerTick,
        Qt::UniqueConnection);
    m_internalBroadcastTimer->start(g_defaultBroadcastCycleMs);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                       START BROADCASTING SERVER ATTRIBUTES
void DefaultLocalConnector::startClearingOutdatedAttributes()
{
    // Broadcast server attributes each g_defaultBroadcastCycleMs
    // milliseconds using UDP service
    m_internalClearOutdatedTimer = std::make_unique<QTimer>();
    connect(
        m_internalClearOutdatedTimer.get(), &QTimer::timeout,
        this,                               &DefaultLocalConnector::clearOutdatedAttributesOnTimerTick,
        Qt::UniqueConnection);
    m_internalClearOutdatedTimer->start(g_defaultBroadcastCycleMs);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                  BROADCAST DATAGRAM WITH SERVER ATTRIBUTES
void DefaultLocalConnector::shareAttributesOnTimerTick()
{
    // Push attributes to the Json bearer string & broadcast it
    ServerData launchedTcpServerAttrib = m_tcpService->getServerAttributes();
    QString serverData(launchedTcpServerAttrib.toJsonQString());
    m_udpService->broadcastDatagram(serverData);
}

// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                           CLEAR OUTDATED SERVER ATTRIBUTES
void DefaultLocalConnector::clearOutdatedAttributesOnTimerTick()
{
    SizeType nowMomentMs = QDateTime::currentMSecsSinceEpoch();
    if(m_discoveredTcpServersAttrib.empty())
    {
        return;
    }
    SizeType oldSize = m_discoveredTcpServersAttrib.size();
    m_discoveredTcpServersAttrib.erase(
                std::remove_if(m_discoveredTcpServersAttrib.begin(),
                               m_discoveredTcpServersAttrib.end(),
                               [nowMomentMs] (ServerData serverData)
                               {
                                   return (nowMomentMs - serverData.m_creationMomentMs) >
                                          g_defaultOutdatingCycleMs;
                               }));
    if (oldSize != m_discoveredTcpServersAttrib.size())
    {
        emit onlineUsersUpdated(getOnlineUsers());
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                             ADD SERVER ATTRIBUTES FROM DISCOVERED DATAGRAM
void DefaultLocalConnector::addServerFromUdpDatagramOnReceive()
{
    ServerData discoveredServerAttributes;
    // Read received datagram and fill it with attributes of the discovered server
    Datagram   datagramContent = m_udpService->getReceivedDatagram();
    discoveredServerAttributes.fromJsonQString(datagramContent.m_data);
    discoveredServerAttributes.m_sourceIp = datagramContent.m_ip;
    // Set timepoint of discovery
    discoveredServerAttributes.m_creationMomentMs =
            QDateTime::currentMSecsSinceEpoch();


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

    emit onlineUsersUpdated(getOnlineUsers());
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
//                                                                        GET CONNECTED USERS
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
//                                                                              START SHARING
void DefaultLocalConnector::startSharing(const QString & userName,
                                         const bool isSelfInitiated)
{
    auto serverAttributesPtr =
            std::find_if(m_discoveredTcpServersAttrib.begin(),
                         m_discoveredTcpServersAttrib.end(),
                         [userName] (const ServerData & serverData)
                         {
                             return serverData.m_name == userName;
                         });
    // Request sharing from user with a given userName
    if (serverAttributesPtr != m_discoveredTcpServersAttrib.end())
    {
        bool isAllowedToStartSharing = true;
        if (isSelfInitiated)
        {
            Message message;
            message.m_type       = MessageType::MessageTypeSharingControlMessage;
            message.m_sourceName = m_tcpService->getServerAttributes().m_name;
            message.m_data       = QString("start");
            isAllowedToStartSharing =
                    m_tcpService->sendDataToTcpServer(message.toJsonQString(),
                                                      *serverAttributesPtr);
        }
        if (isAllowedToStartSharing)
        {
            m_connectedTcpServersAttrib.push_back(*serverAttributesPtr);

            qDebug() << "sharing was started";
            emit connectedUsersUpdated(getConnectedUsers());
        }
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                               STOP SHARING
void DefaultLocalConnector::stopSharing(const QString & userName,
                                        const bool isSelfInitiated)
{
    auto serverAttributesPtr =
            std::find_if(m_connectedTcpServersAttrib.begin(),
                         m_connectedTcpServersAttrib.end(),
                         [userName] (const ServerData & serverData)
                         {
                             return serverData.m_name == userName;
                         });

    // Request sharing from user with a given userName
    if (serverAttributesPtr != m_connectedTcpServersAttrib.end())
    {
        if (isSelfInitiated)
        {
            Message message;
            message.m_type       = MessageType::MessageTypeSharingControlMessage;
            message.m_sourceName = m_tcpService->getServerAttributes().m_name;
            message.m_data       = QString("stop");
            m_tcpService->sendDataToTcpServer(message.toJsonQString(),
                                              *serverAttributesPtr);
        }

        m_connectedTcpServersAttrib.erase(serverAttributesPtr, std::next(serverAttributesPtr));

        qDebug() << "sharing was stopped";
        emit connectedUsersUpdated(getConnectedUsers());
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                      SHARE MESSAGE BETWEEN CONNECTED USERS
void DefaultLocalConnector::shareMessage(const QString & messageContent)
{
    Message message;
    message.m_type       = MessageType::MessageTypeChatMessage;
    message.m_sourceName = m_tcpService->getServerAttributes().m_name;
    message.m_data       = messageContent;
    for(const auto & serverAttrib : m_connectedTcpServersAttrib)
    {
        m_tcpService->sendDataToTcpServer(message.toJsonQString(),
                                          serverAttrib);
    }
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
    case MessageType::MessageTypeSharingControlMessage:
        if (message.m_data == "start")
        {
            startSharing(message.m_sourceName, false);

            qDebug() << message.m_sourceName << " wants to start share his workflow";
            emit startSharingRequested(message.m_sourceName);
        }
        if (message.m_data == "stop")
        {
            stopSharing(message.m_sourceName, false);

            qDebug() << message.m_sourceName << " wants to stop share his workflow";
            emit stopSharingRequested(message.m_sourceName);
        }
        break;
    case MessageType::MessageTypeChatMessage:
        qDebug() << message.m_sourceName << " shared his message";
        emit messageReceived(message.m_sourceName, message.m_data);
        break;
    case MessageType::MessageTypeChangesMessage:
        qDebug() << message.m_sourceName << " shared his change";
        // TODO on second sprint
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

    if(m_tcpService->sendDataToTcpServer(message.toJsonQString(), serverData))
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
