#include "tcpservice.h"
// ==========================================================================================
// ==========================================================================================
//                                                                    TCP SERVICE CONSTRUCTOR
TcpService::TcpService(const QString & serverName, QObject *qObject) :
    QObject(qObject), mcServerPortNumber(gDefaultTcpPortNumber), mcServerName(serverName)
{
    QNetworkConfigurationManager confManager;
    // Check if additional network configuration is needed
    bool isNetConfigurationNeeded =
            (confManager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired);
    if (isNetConfigurationNeeded)
    {
        // Get previously saved network configuration from the registry
        QSettings netSettings;
        netSettings.beginGroup(QString("PSnet"));
        const QString configId =
                netSettings.value(QString("DefaultNetworkConfiguration")).toString();
        netSettings.endGroup();

        // Try to discover configuration or use system's default
        QNetworkConfiguration configData =
                confManager.configurationFromIdentifier(configId);
        if (QNetworkConfiguration::Discovered !=
                (configData.state() & QNetworkConfiguration::Discovered))
        {
            configData = confManager.defaultConfiguration();
        }

        // Start opening new session with existent configurations
        mpNetSession = std::make_unique<QNetworkSession>(configData, this);

        // Configure server when session opening succeeds
        connect(
            mpNetSession.get(), &QNetworkSession::opened,
            this,               &TcpService::configureServerOnReady,
            Qt::UniqueConnection);

        mpNetSession->open();
    }
    else
    {
        // Configure server immediately if additional
        // network configuration is not needed
        configureServerOnReady();
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                       CONFIGURE TCP SERVER
void TcpService::configureServerOnReady()
{
    // Case, if additional configuration has been previously done
    if (mpNetSession)
    {
        QNetworkConfiguration configData = mpNetSession->configuration();
        QString configId = (configData.type() == QNetworkConfiguration::UserChoice) ?
            mpNetSession->sessionProperty(QString("UserChoiceConfiguration")).toString() :
            configData.identifier();

        // Save network configurations to the registry
        QSettings netSettings;
        netSettings.beginGroup(QString("PSnet"));
        netSettings.setValue(QString("DefaultNetworkConfiguration"), configId);
        netSettings.endGroup();
    }

    mpTcpServer = std::make_unique<QTcpServer>(new QTcpServer(this));
    if (!mpTcpServer->listen(QHostAddress::Any, mcServerPortNumber))
    {
        mpTcpServer->close();
        return;
    }
    saveServerAttributes();

    // Trigger processing of the requested connections
    connect(
        mpTcpServer.get(), &QTcpServer::newConnection,
        this,              &TcpService::attachSocketOnClientConnected,
        Qt::UniqueConnection);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                       SAVE SERVER ATTRIBUTES INSIDE SERVER
void TcpService::saveServerAttributes()
{
    // Insert server name and server port
    mServerAttributes.mName = mcServerName;
    mServerAttributes.mPort = mpTcpServer->serverPort();

    // Append server addresses for each interface
    for (const auto & availInterface : QNetworkInterface::allInterfaces())
    {
        QNetworkInterface::InterfaceFlags intFlags = availInterface.flags();
        bool isInterfaceRunning (intFlags & QNetworkInterface::IsRunning);
        bool isInterfaceLoopBack(intFlags & QNetworkInterface::IsLoopBack);
        // Omit interfaces that are LoopBack or are not active
        if(!isInterfaceRunning || isInterfaceLoopBack) continue;

        // Add all active IPv4 addresses inside this interface
        for (const auto & ipEntry : availInterface.addressEntries())
        {
            if(ipEntry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                mServerAttributes.mIps.push_back(ipEntry.ip());
            }
        }
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                           CONNECT TO TCP SERVER BY GIVEN SERVER ATTRIBUTES
bool TcpService::sendDataToTcpServer(const QString & data, const ServerData & serverData)
{
    if (serverData.empty())
    {
        return false;
    }

    // Create new empty server socket & attach it after successful connection
    QTcpSocket * pServerSocket(new QTcpSocket(this));
    connect(
        pServerSocket, &QTcpSocket::connected,
        this,          &TcpService::attachSocketOnServerConnected,
        Qt::AutoConnection);

    // Set server port and its working IP address from interface IPs
    const PortNumType port = serverData.mPort;
    QHostAddress serverIp  = serverData.mActiveIp;
    for (const auto & interfaceIp : serverData.mIps)
    {
        if (interfaceIp.isEqual(serverIp, QHostAddress::ConvertV4MappedToIPv4))
        {
            serverIp = interfaceIp;
            break;
        }
    }

    // Try to connect to socket and then send data
    pServerSocket->connectToHost(serverIp, port);
    if (pServerSocket->waitForConnected(gDefaultConnTimeoutMs))
    {
        pServerSocket->write(data.toUtf8());
        pServerSocket->flush();
        return true;
    }
    return false;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                              RETURN SERVER ACTIVITY STATUS
bool TcpService::isServerActive() const
{
    return mpTcpServer->isListening();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                      GET SERVER ATTRIBUTES
ServerData TcpService::getServerAttributes() const
{
    return mServerAttributes;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                  GET LAST RECEIVED SEGMENT
Segment TcpService::getReceivedSegment() const
{
    return mPendingSegment;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                   SAVE SEGMENT ON RECEIVAL
void TcpService::saveSegmentOnReceival()
{
    // Get source' socket ptr
    QTcpSocket * pSenderSocket(qobject_cast<QTcpSocket*>(sender()));

    // Get received data & socket attributes, i.e. IP and port
    QByteArray   data (pSenderSocket->readAll());
    QHostAddress ip   (pSenderSocket->peerAddress());
    PortNumType  port (pSenderSocket->peerPort());

    // Save attributes of the segment
    mPendingSegment.mContent = data;
    mPendingSegment.mIp      = ip;
    mPendingSegment.mPort    = port;

    pSenderSocket->disconnectFromHost();

    // Inform about segment receival
    emit newSegmentSaved();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                          ATTACH SERVER SOCKET ON CONNECTED
void TcpService::attachSocketOnServerConnected()
{
    // Get connected host' socket ptr
    QTcpSocket * pServerSocket(qobject_cast<QTcpSocket*>(sender()));

    // Trigger saving received segments on readyRead
    connect(
        pServerSocket, &QTcpSocket::readyRead,
        this,          &TcpService::saveSegmentOnReceival,
        Qt::AutoConnection);

    // Trigger informing about disconnected hosts
    connect(
        pServerSocket, &QTcpSocket::disconnected,
        this,          &TcpService::closeSocketOnDisconnected,
        Qt::AutoConnection);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                          ATTACH CLIENT SOCKET ON CONNECTED
void TcpService::attachSocketOnClientConnected()
{
    // Get brand new client' socket and save its ptr
    QTcpSocket * pClientSocket(mpTcpServer->nextPendingConnection());

    // Trigger saving received segments on readyRead
    connect(
        pClientSocket, &QTcpSocket::readyRead,
        this,          &TcpService::saveSegmentOnReceival,
        Qt::AutoConnection);

    // Trigger informing about disconnected hosts
    connect(
        pClientSocket, &QTcpSocket::disconnected,
        this,          &TcpService::closeSocketOnDisconnected,
        Qt::AutoConnection);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                               CLOSE SOCKET ON DISCONNECTED
void TcpService::closeSocketOnDisconnected()
{
    // Get and close disconnected socket
    QTcpSocket * pDisconnectedSocket(qobject_cast<QTcpSocket*>(sender()));
    pDisconnectedSocket->close();
}// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                     TCP SERVICE DESTRUCTOR
TcpService::~TcpService()
{
    if (mpTcpServer->isListening())
    {
        // Close server only if it has been launched
        mpTcpServer->close();
    }
}
