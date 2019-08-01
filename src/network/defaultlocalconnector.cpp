#include "defaultlocalconnector.h"
// ==========================================================================================
// ==========================================================================================
//                                                             LAUNCH SERVICES ON USER LOG IN
void DefaultLocalConnector::configureOnLogin(const QString & userName)
{
    // Return if services has previously been configured
    if (mpUdpService && mpTcpService)
    {
        return;
    }
    // Configure the UDP service & TCP service
    mpUdpService = std::unique_ptr<UdpService>(new UdpService());
    mpTcpService = std::unique_ptr<TcpService>(new TcpService(userName));

    emit serviceStatus(mpTcpService->isServerActive()? true : false);
    if (!mpTcpService->isServerActive())
    {
        return;
    }

    // Begin discovering neighbor servers using UDP service
    connect(
        mpUdpService.get(), &UdpService::newDatagramSaved,
        this,               &DefaultLocalConnector::addServerAttributesOnReceive,
        Qt::UniqueConnection);
    // Begin saving incoming messages using TCP service
    connect(
        mpTcpService.get(), &TcpService::newSegmentSaved,
        this,               &DefaultLocalConnector::processTcpSegmentOnReceive,
        Qt::UniqueConnection
    );

    startSharingAttributes();
    startClearingOutdatedAttributes();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                       START BROADCASTING SERVER ATTRIBUTES
void DefaultLocalConnector::startSharingAttributes()
{
    // Broadcast server attributes each gDefaultBroadcastCycleMs milliseconds
    mpBroadcastingTimer = std::make_unique<QTimer>();
    connect(
        mpBroadcastingTimer.get(), &QTimer::timeout,
        this,                      &DefaultLocalConnector::shareAttributesOnTimerTick,
        Qt::UniqueConnection);
    mpBroadcastingTimer->start(gDefaultBroadcastCycleMs);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                  BROADCAST DATAGRAM WITH SERVER ATTRIBUTES
void DefaultLocalConnector::shareAttributesOnTimerTick()
{
    // Push attributes to the Json bearer string & broadcast it
    ServerData launchedTcpServerAttrib = mpTcpService->getServerAttributes();
    QString serverData(launchedTcpServerAttrib.toJsonQString());
    mpUdpService->broadcastDatagram(serverData);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                         START CLEARING OUTDATED ATTRIBUTES
void DefaultLocalConnector::startClearingOutdatedAttributes()
{
    // Clean outdated attributes each gDefaultOutdatingCycleMs milliseconds
    mAttributesObsoletionTimer = std::make_unique<QTimer>();
    connect(
        mAttributesObsoletionTimer.get(), &QTimer::timeout,
        this,                             &DefaultLocalConnector::clearOutdatedAttributesOnTimerTick,
        Qt::UniqueConnection);
    mAttributesObsoletionTimer->start(gDefaultOutdatingCycleMs);
}

// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                           CLEAR OUTDATED SERVER ATTRIBUTES
void DefaultLocalConnector::clearOutdatedAttributesOnTimerTick()
{
    if(mDiscoveredServersAttrib.empty())
    {
        return;
    }

    SizeType discoveredServersNumBefore = mDiscoveredServersAttrib.size();
    SizeType currentMs = QDateTime::currentMSecsSinceEpoch();
    mDiscoveredServersAttrib.erase(
                std::remove_if(mDiscoveredServersAttrib.begin(),
                               mDiscoveredServersAttrib.end(),
                               [currentMs] (const ServerData & serverAttributes)
                               {
                                   return (currentMs - serverAttributes.mDiscoveryMoment) >
                                           gDefaultOutdatingCycleMs;
                               }),
                mDiscoveredServersAttrib.end());
    SizeType discoveredServersNumAfter = mDiscoveredServersAttrib.size();

    if (discoveredServersNumAfter < discoveredServersNumBefore)
    {
        emit onlineUsersUpdated(getOnlineUsers());
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                             ADD SERVER ATTRIBUTES FROM DISCOVERED DATAGRAM
void DefaultLocalConnector::addServerAttributesOnReceive()
{
    Datagram datagramContent = mpUdpService->getReceivedDatagram();

    ServerData curretnAttributes;
    curretnAttributes.fromJsonQString(datagramContent.mContent);
    // Explicitly set timepoint of discovery and active ip
    curretnAttributes.mDiscoveryMoment = QDateTime::currentMSecsSinceEpoch();
    curretnAttributes.mActiveIp        = datagramContent.mIp;


    if (curretnAttributes.empty())
    {
        // Datagram was corrupted
        return;
    }
    if (curretnAttributes.mName == mpTcpService->getServerAttributes().mName)
    {
        // Discovered server name matches current server name
        return;
    }
    auto pExistentAttributes =
            std::find_if(mDiscoveredServersAttrib.begin(),
                         mDiscoveredServersAttrib.end(),
                         [curretnAttributes] (const ServerData & inServerData)
                         {
                             return inServerData.mName ==
                                    curretnAttributes.mName;
                         });
    if (pExistentAttributes != mDiscoveredServersAttrib.cend())
    {
        // Server with a given name has already been discovered so update discovery moment
        pExistentAttributes->mDiscoveryMoment = curretnAttributes.mDiscoveryMoment;
        return;
    }

    // Finally, save information about server & emit user list updated signal
    mDiscoveredServersAttrib.push_back(curretnAttributes);
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
    for (const auto & serverAttrib : mDiscoveredServersAttrib)
        userNames.push_back(serverAttrib.mName);

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
    for (const auto & serverAttrib : mConnectedServersAttrib)
        userNames.push_back(serverAttrib.mName);

    return userNames;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                   START SHARING ON REQUEST
void DefaultLocalConnector::startSharingOnCommand(const QString userName)
{
    startSharing(userName, true);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                    STOP SHARING ON REQUEST
void DefaultLocalConnector::stopSharingOnCommand(const QString userName)
{
    stopSharing(userName, true);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                              START SHARING
void DefaultLocalConnector::startSharing(const QString userName,
                                         const bool isSelfInitiated)
{
    // Check if user is already connected
    auto connectedServerAttributesPtr =
            std::find_if(mConnectedServersAttrib.begin(),
                         mConnectedServersAttrib.end(),
                         [userName] (const ServerData & serverData)
                         {
                             return serverData.mName == userName;
                         });
    if (connectedServerAttributesPtr != mConnectedServersAttrib.end())
    {
        return;
    }

    auto serverAttributesPtr =
            std::find_if(mDiscoveredServersAttrib.begin(),
                         mDiscoveredServersAttrib.end(),
                         [userName] (const ServerData & serverData)
                         {
                             return serverData.mName == userName;
                         });
    // Request sharing from user with a given userName
    if (serverAttributesPtr != mDiscoveredServersAttrib.end())
    {
        bool isAllowedToStartSharing = true;
        if (isSelfInitiated)
        {
            Message message;
            message.mType       = Message::Type::SharingControlMessage;
            message.mSourceName = mpTcpService->getServerAttributes().mName;
            message.mContent       = QString("start");
            isAllowedToStartSharing =
                    mpTcpService->sendDataToTcpServer(message.toJsonQString(),
                                                      *serverAttributesPtr);
        }
        if (isAllowedToStartSharing)
        {
            mConnectedServersAttrib.push_back(*serverAttributesPtr);

            qDebug() << "sharing was started";
            emit connectedUsersUpdated(getConnectedUsers());
        }
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                               STOP SHARING
void DefaultLocalConnector::stopSharing(const QString userName,
                                        const bool isSelfInitiated)
{
    auto serverAttributesPtr =
            std::find_if(mConnectedServersAttrib.begin(),
                         mConnectedServersAttrib.end(),
                         [userName] (const ServerData & serverData)
                         {
                             return serverData.mName == userName;
                         });

    // Request sharing from user with a given userName
    if (serverAttributesPtr != mConnectedServersAttrib.end())
    {
        if (isSelfInitiated)
        {
            Message message;
            message.mType       = Message::Type::SharingControlMessage;
            message.mSourceName = mpTcpService->getServerAttributes().mName;
            message.mContent       = QString("stop");
            mpTcpService->sendDataToTcpServer(message.toJsonQString(),
                                              *serverAttributesPtr);
        }

        mConnectedServersAttrib.erase(serverAttributesPtr);

        qDebug() << "sharing was stopped";
        emit connectedUsersUpdated(getConnectedUsers());
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                      SHARE MESSAGE BETWEEN CONNECTED USERS
void DefaultLocalConnector::shareMessage(const QString messageContent)
{
    Message message;
    message.mType       = Message::Type::ChatMessage;
    message.mSourceName = mpTcpService->getServerAttributes().mName;
    message.mContent       = messageContent;
    for(const auto & serverAttrib : mConnectedServersAttrib)
    {
        mpTcpService->sendDataToTcpServer(message.toJsonQString(),
                                          serverAttrib);
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                             TRANSLATE RECEIVED TCP SEGMENT
void DefaultLocalConnector::processTcpSegmentOnReceive()
{
    Segment segment = mpTcpService->getReceivedSegment();
    Message message;
    message.fromJsonQString(segment.mContent);

    if (message.empty())
    {
        return;
    }

    switch(message.mType)
    {
    case Message::Type::SharingControlMessage:
        if (message.mContent == "start")
        {
            startSharing(message.mSourceName, false);

            qDebug() << message.mSourceName << " wants to start share his workflow";
            emit startSharingRequested(message.mSourceName);
        }
        if (message.mContent == "stop")
        {
            stopSharing(message.mSourceName, false);

            qDebug() << message.mSourceName << " wants to stop share his workflow";
            emit stopSharingRequested(message.mSourceName);
        }
        break;
    case Message::Type::ChatMessage:
        qDebug() << message.mSourceName << " shared his message";
        emit messageReceived(message.mSourceName, message.mContent);
        break;
    case Message::Type::ChangesMessage:
        qDebug() << message.mSourceName << " shared his change";
        // TODO on second sprint
        break;
    }

#ifdef CUSTOM_DEBUG
    qDebug() << "____________________________________________________________";
    qDebug() << "TCP SEGMENT HAS BEEN RECEIVED: ";
    qDebug() << "data ->" << segment.mContent;
    qDebug() << "ip -> "  << segment.mIp.toString();
    qDebug() << "port ->" << segment.mPort;
    qDebug() << "____________________________________________________________";
#endif // CUSTOM_DEBUG
}



#ifdef CUSTOM_DEBUG

void DefaultLocalConnector::testSendHelloToLastServer()
{
    ServerData serverData;
    if(!mDiscoveredServersAttrib.empty())
    {
        serverData = mDiscoveredServersAttrib.back();
    }

    qDebug() << "try to send hello to server " << serverData.mName;

    QString data("hello from ");
    data.append(mpTcpService->getServerAttributes().mName);
    data.append(" to ");
    data.append(serverData.mName);

    Message message;
    message.mContent = data;
    message.mType = Message::Type::ChatMessage;
    message.mSourceName = mpTcpService->getServerAttributes().mName;

    if(mpTcpService->sendDataToTcpServer(message.toJsonQString(), serverData))
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
