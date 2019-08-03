#include "defaultlocalconnector.h"
// ==========================================================================================
// ==========================================================================================
//                                                             LAUNCH SERVICES ON USER LOG IN
void DefaultLocalConnector::configureOnLogin(const QString & userName)
{
    if (mpUdpService && mpTcpService)
    {
        // If services has previously been configured
        emit serviceStatusChanged(mpTcpService->isServerActive()? true : false);
        return;
    }
    // Configure the UDP service & TCP service
    mpUdpService = std::unique_ptr<UdpService>(new UdpService());
    mpTcpService = std::unique_ptr<TcpService>(new TcpService(userName));

    emit serviceStatusChanged(mpTcpService->isServerActive()? true : false);
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
        this,               &DefaultLocalConnector::parseTcpSegmentOnReceive,
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

    QStringList outdatedServerNames;
    SizeType currentMs = QDateTime::currentMSecsSinceEpoch();
    mDiscoveredServersAttrib.erase(
                std::remove_if(mDiscoveredServersAttrib.begin(),
                               mDiscoveredServersAttrib.end(),
                               [currentMs, &outdatedServerNames] (const ServerData & serverAttributes) -> bool
                               {
                                   bool isOutdated = (currentMs - serverAttributes.mDiscoveryMoment) >
                                                      gDefaultOutdatingCycleMs;
                                   if (isOutdated)
                                   {
                                       outdatedServerNames.push_back(serverAttributes.mName);
                                   }
                                   return isOutdated;
                               }),
                mDiscoveredServersAttrib.end());

    // If outdated attributes have been removed, inform GUI
    if (!outdatedServerNames.empty())
    {
        bool isAnyConnectedAttributesOutdated = false;
        for(const auto & serverName : outdatedServerNames)
        {
            if(!popFromConnectedAttributes(serverName).empty())
            {
                isAnyConnectedAttributesOutdated = true;
            }
        }
        if (isAnyConnectedAttributesOutdated)
        {
            emit connectedUsersUpdated(getConnectedUsers());
        }
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
//                                                             TRY ADDING SERVER TO CONNECTED
ServerData DefaultLocalConnector::pushToConnectedAttributes(const QString & serverName)
{
    ServerData serverAttributes;
    auto pDiscoveredServerAttributes =
            std::find_if(mDiscoveredServersAttrib.begin(),
                         mDiscoveredServersAttrib.end(),
                         [serverName] (const ServerData & serverData)
                         {
                             return serverData.mName == serverName;
                         });
    if(pDiscoveredServerAttributes == mDiscoveredServersAttrib.end())
    {
        // If there is no such server in the discovered serers
        return serverAttributes;
    }
    auto pConnectedServerAttributes =
            std::find_if(mConnectedServersAttrib.begin(),
                         mConnectedServersAttrib.end(),
                         [serverName] (const ServerData & serverData)
                         {
                             return serverData.mName == serverName;
                         });
    if(pConnectedServerAttributes == mConnectedServersAttrib.end())
    {
        // If server is not already connected
        serverAttributes = *pDiscoveredServerAttributes;
        mConnectedServersAttrib.push_back(serverAttributes);
    }
    return serverAttributes;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                         TRY REMOVING SERVER FROM CONNECTED
ServerData DefaultLocalConnector::popFromConnectedAttributes(const QString & serverName)
{
    ServerData serverAttributes;
    auto pServerAttributes =
            std::find_if(mConnectedServersAttrib.begin(),
                         mConnectedServersAttrib.end(),
                         [serverName] (const ServerData & serverData)
                         {
                             return serverData.mName == serverName;
                         });
    if(pServerAttributes != mConnectedServersAttrib.end())
    {
        serverAttributes = *pServerAttributes;
        mConnectedServersAttrib.erase(pServerAttributes);
    }
    return serverAttributes;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                              START SHARING
void DefaultLocalConnector::startSharing(const QString userName)
{
    if (!mpUdpService || !mpTcpService)
    {
        // If services has not been configured
        return;
    }
    ServerData serverAttributes = pushToConnectedAttributes(userName);
    if (!serverAttributes.empty())
    {
        Message message;
        message.mType       = Message::Type::StartSharingMessage;
        message.mSourceName = mpTcpService->getServerAttributes().mName;
        mpTcpService->sendDataToTcpServer(message.toJsonQString(), serverAttributes);

        emit connectedUsersUpdated(getConnectedUsers());
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                               STOP SHARING
void DefaultLocalConnector::stopSharing(const QString userName)
{
    if (!mpUdpService || !mpTcpService)
    {
        // If services has not been configured
        return;
    }
    ServerData serverAttributes = popFromConnectedAttributes(userName);
    if (!serverAttributes.empty())
    {
        Message message;
        message.mType       = Message::Type::StopSharingMessage;
        message.mSourceName = mpTcpService->getServerAttributes().mName;
        mpTcpService->sendDataToTcpServer(message.toJsonQString(), serverAttributes);

        emit connectedUsersUpdated(getConnectedUsers());
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                      SHARE MESSAGE BETWEEN CONNECTED USERS
void DefaultLocalConnector::shareMessage(const QString messageContent)
{
    if (!mpUdpService || !mpTcpService)
    {
        // If services has not been configured
        return;
    }
    Message message;
    message.mType       = Message::Type::ChatMessage;
    message.mSourceName = mpTcpService->getServerAttributes().mName;
    message.mContent    = messageContent;
    for(const auto & serverAttrib : mConnectedServersAttrib)
    {
        mpTcpService->sendDataToTcpServer(message.toJsonQString(), serverAttrib);
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                       SHARE CHANGE BETWEEN CONNECTED USERS
void DefaultLocalConnector::shareChange(const QString changeContent)
{
    if (!mpUdpService || !mpTcpService)
    {
        // If services has not been configured
        return;
    }
    Message message;
    message.mType       = Message::Type::ChangesMessage;
    message.mSourceName = mpTcpService->getServerAttributes().mName;
    message.mContent    = changeContent;
    for(const auto & serverAttrib : mConnectedServersAttrib)
    {
        mpTcpService->sendDataToTcpServer(message.toJsonQString(), serverAttrib);
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                            COORDINATE RECEIVED TCP SEGMENT
void DefaultLocalConnector::parseTcpSegmentOnReceive()
{
    Segment segment = mpTcpService->getReceivedSegment();
    Message message;
    message.fromJsonQString(segment.mContent);

    if (message.empty())
    {
        // If message is corrupted
        return;
    }

    switch(message.mType)
    {
    case Message::Type::StartSharingMessage:
        qDebug() << message.mSourceName << " wants to start sharing his workflow";
        if(!pushToConnectedAttributes(message.mSourceName).empty())
        {
            // Emit signal if user is successfully connected
            emit startSharingRequested (message.mSourceName);
            emit connectedUsersUpdated(getConnectedUsers());
        }
        break;
    case Message::Type::StopSharingMessage:
        qDebug() << message.mSourceName << " wants to stop sharing his workflow";
        if(!popFromConnectedAttributes(message.mSourceName).empty())
        {
            // Emit signal if user is successfully disconnected
            emit stopSharingRequested (message.mSourceName);
            emit connectedUsersUpdated(getConnectedUsers());
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
}
