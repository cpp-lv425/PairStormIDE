#include "tcpservice.h"
// ==========================================================================================
// ==========================================================================================
//                                                                    TCP SERVICE CONSTRUCTOR
TcpService::TcpService(const QString & serverName, QObject *qObject) :
    QObject(qObject), m_serverName(serverName), m_portNumber(g_defaultTcpPortNumber)
{
    QNetworkConfigurationManager confManager;
    // Check if additional network configuration is needed
    bool isNetConfigurationNeeded =
            (confManager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired);
    if (isNetConfigurationNeeded)
    {
        // Get previously saved network configuration from the registry
        QSettings netSettings(QSettings::UserScope, g_orgLabel, g_appLabel);
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
        m_netSessionPtr = std::make_unique<QNetworkSession>(configData, this);

        // Configure server when session opening succeeds
        connect(
            m_netSessionPtr.get(), &QNetworkSession::opened,
            this,               &TcpService::configureServer,
            Qt::UniqueConnection);

        m_netSessionPtr->open();
    }
    else
    {
        // Configure server immediately if additional
        // network configuration is not needed
        configureServer();
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                       CONFIGURE TCP SERVER
void TcpService::configureServer()
{
    // Case, if additional configuration has been previously done
    if (m_netSessionPtr)
    {
        QNetworkConfiguration configData = m_netSessionPtr->configuration();
        QString configId = (configData.type() == QNetworkConfiguration::UserChoice) ?
            m_netSessionPtr->sessionProperty(QString("UserChoiceConfiguration")).toString() :
            configData.identifier();

        // Save network configurations to the registry
        QSettings settings(QSettings::UserScope, g_orgLabel, g_appLabel);
        settings.beginGroup(QString("PSnet"));
        settings.setValue(QString("DefaultNetworkConfiguration"), configId);
        settings.endGroup();
    }

    // Launch QTcpServer on the m_portNumber
    m_tcpServerPtr = std::make_unique<QTcpServer>(new QTcpServer(this));
    if (!m_tcpServerPtr->listen(QHostAddress::Any, m_portNumber))
    {
        qDebug() << "Failure: server hasn't been started";
        m_tcpServerPtr->close();
        return;
    }
    qDebug() << "Success: server has been started on port: " << m_tcpServerPtr->serverPort();

    // Fill server attributes
    m_serverAttributes = resolveServerAttributes();

    // Trigger processing of the connections, requested by clients
    connect(
        m_tcpServerPtr.get(), &QTcpServer::newConnection,
        this,                 &TcpService::configureSocketOnClientConnection,
        Qt::UniqueConnection);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                   GET INSTANCE OF THE TCP SERVICE PROVIDER
std::shared_ptr<TcpService> TcpService::getService(const QString & serverName)
{
    static std::shared_ptr<TcpService>
            serviceInstance(new TcpService(serverName));
    return serviceInstance;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                     TCP SERVICE DESTRUCTOR
TcpService::~TcpService()
{
    // Close launched server
    m_tcpServerPtr->close();

    // Disconnect active sockets
    std::for_each(m_connectedSockets.begin(), m_connectedSockets.end(),
                  [this](std::shared_ptr<QTcpSocket> socket)
                  {
                      disconnectSocket(socket);
                  });
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                            GET NAME OF THE LAUNCHED SERVER
ServerData TcpService::getServerAttributes() const
{
    return m_serverAttributes;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                 GET CONFIGURATIONS OF THE LOCAL TCP SERVER
ServerData TcpService::resolveServerAttributes() const
{
    ServerData serverData;

    // Insert server name and server port
    serverData.m_name = m_serverName;
    serverData.m_port = m_tcpServerPtr->serverPort();

    // Append server addresses for each interface
    for (const auto & availableInt : QNetworkInterface::allInterfaces())
    {
        QNetworkInterface::InterfaceFlags intFlags = availableInt.flags();
        bool isInterfaceRunning(intFlags & QNetworkInterface::IsRunning);
        bool isInterfaceLoopBack(intFlags & QNetworkInterface::IsLoopBack);
        // Omit interfaces that are LoopBack or are not active
        if(!isInterfaceRunning || isInterfaceLoopBack) continue;

        // Add all active IPv4 addresses inside this interface
        for (const auto & ipEntry : availableInt.addressEntries())
        {
            if(ipEntry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                serverData.m_ips.push_back(ipEntry.ip());
            }
        }
    }

    return serverData;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                           CONNECT TO TCP SERVER BY GIVEN SERVER ATTRIBUTES
void TcpService::connectToTcpServer(const ServerData & serverData)
{
    // Create new empty server socket & save its ptr
    std::shared_ptr<QTcpSocket>
            serverSocketPtr(new QTcpSocket(this));

    serverSocketPtr->abort();

    // Force preliminary configuration in case of successful connection
    connect(
        serverSocketPtr.get(), &QTcpSocket::connected,
        this,                  &TcpService::configureSocketOnServerConnection,
        Qt::AutoConnection);

    // Try to connect using present server' ip addresses
    const PortNumType  port = serverData.m_port;
    const QHostAddress ip =   serverData.m_sourceIp;

    serverSocketPtr->connectToHost(ip, port);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                 CONFIGURE SOCKET WHEN SERVER HAS CONNECTED
void TcpService::configureSocketOnServerConnection()
{
    // Get connected host' socket ptr
    std::shared_ptr<QTcpSocket>
            serverSocketPtr(qobject_cast<QTcpSocket*>(sender()));
    m_connectedSockets.push_back(serverSocketPtr);

    // Trigger saving received segments on readyRead
    connect(
        serverSocketPtr.get(), &QTcpSocket::readyRead,
        this,                  &TcpService::saveSegmentOnReceival,
        Qt::AutoConnection);

    // Trigger informing about disconnected hosts
    connect(
        serverSocketPtr.get(), &QTcpSocket::disconnected,
        this,                  &TcpService::removeSocketOnDisconnected,
        Qt::AutoConnection);




    // TODO establish normal connection
    // TODO write userName -> socket relation
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                 CONFIGURE SOCKET WHEN CLIENT HAS CONNECTED
void TcpService::configureSocketOnClientConnection()
{
        qDebug() << "Host request connection:";



    // Get brand new client' socket and save its ptr
    std::shared_ptr<QTcpSocket>
            clientSocketPtr(m_tcpServerPtr->nextPendingConnection());
    m_connectedSockets.push_back(clientSocketPtr);

    // Trigger saving received segments on readyRead
    connect(
        clientSocketPtr.get(), &QTcpSocket::readyRead,
        this,                  &TcpService::saveSegmentOnReceival,
        Qt::AutoConnection);

    // Trigger informing about disconnected hosts
    connect(
        clientSocketPtr.get(), &QTcpSocket::disconnected,
        this,                  &TcpService::removeSocketOnDisconnected,
        Qt::AutoConnection);


    //===================================================================================
    //                                                                ATTENTION: HARDCODE


    qDebug() << "ip: " << clientSocketPtr->peerAddress();
    qDebug() << "port: " << clientSocketPtr->peerPort();

    //===================================================================================



    emit socketConnected(clientSocketPtr);

    // TODO establish normal connection
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                             SEND DATA THROUGH GIVEN SOCKET
void TcpService::sendThroughSocket(const QString & data,
                                   std::shared_ptr<QTcpSocket> receiver)
{
    // Create & fill TCP segment with data
    QByteArray segment;
    segment.append(data);

    // Send TCP segment through socket
    receiver->write(segment);
    receiver->flush();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                 SAVE EACH RECEIVED SEGMENT
void TcpService::saveSegmentOnReceival()
{
    // Get source' socket ptr
    std::shared_ptr<QTcpSocket>
            senderSocketPtr(qobject_cast<QTcpSocket*>(sender()));

    // Get received data & socket attributes, i.e. IP and port
    QByteArray   data (senderSocketPtr->readAll());
    QHostAddress ip   (senderSocketPtr->peerAddress());
    PortNumType  port (senderSocketPtr->peerPort());

    // Save attributes of the segment
    m_pendingSegment.m_data = data;
    m_pendingSegment.m_ip   = ip;
    m_pendingSegment.m_port = port;

    // Inform about segment receival
    emit newSegmentSaved();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                  GET LAST RECEIVED SEGMENT
Segment TcpService::getReceivedSegment() const
{
    return m_pendingSegment;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                    DISCONNECT GIVEN SOCKET
void TcpService::disconnectSocket(std::shared_ptr<QTcpSocket> socket)
{
    socket->disconnectFromHost();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                             REMOVE DISCONNECTED SOCKET FROM ACTIVE SOCKETS
void TcpService::removeSocketOnDisconnected()
{
    // Get disconnected socket
    std::shared_ptr<QTcpSocket>
            disconnectedSocketPtr(qobject_cast<QTcpSocket*>(sender()));

    // Suppose that disconnected socket name is unknown
    QString disconnectedSocketName("Unknown");
    // Try to get real name of the server, described by the given socket
    bool isNameResolved =
            resolveUserNameBySocket(disconnectedSocketPtr, disconnectedSocketName);
    if (isNameResolved)
    {
        removeUserNameSocketRelation(disconnectedSocketName);
    }

    // Move socket away from active sockets
    auto newConnectedSocketsEnd =
            std::remove(m_connectedSockets.begin(),
                        m_connectedSockets.end(),
                        disconnectedSocketPtr);

    // Close the socket
    std::for_each(newConnectedSocketsEnd,
                  m_connectedSockets.end(),
                  [] (std::shared_ptr<QTcpSocket> socket)
                  {
                      socket->close();
                  });

    // Erase socket from active sockets
    m_connectedSockets.erase(newConnectedSocketsEnd);



#ifdef CUSTOM_DEBUG
    qDebug() << "____________________________________________________________";
    qDebug() << "Host has been disconnected:";
    qDebug() << "    ip   -> " << disconnectedSocketPtr->peerAddress().toString();
    qDebug() << "    port -> " << disconnectedSocketPtr->peerPort();
    qDebug() << "____________________________________________________________";
#endif // CUSTOM_DEBUG



    // Inform about socket disconnection
    emit socketDisconnected(disconnectedSocketName);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                        INSERT USERNAME TO SOCKET RELATION
void TcpService::setUserNameSocketRelation(const std::shared_ptr<QTcpSocket> & userSocket,
                                           const QString & userName)
{
    m_nameToSocket.insert(std::make_pair(userName.toStdString(), userSocket));
    //m_nameToSocket[userName.toStdString()] = userSocket;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                         REMOVE USERNAME TO SOCKET RELATION
void TcpService::removeUserNameSocketRelation(const QString & userName)
{
    //m_nameToSocket.erase(m_nameToSocket.find(userName.toStdString()));
    m_nameToSocket.erase(userName.toStdString());
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                      GET SOCKET OUT OF THE GIVEN USER NAME
bool TcpService::resolveSocketByUserName(std::shared_ptr<QTcpSocket> & userSocket,
                                         const QString & userName)
{
    // Try to find user name
    const auto socketPtr = m_nameToSocket.find(userName.toStdString());
    // Return false if not found
    if(socketPtr == m_nameToSocket.end())
    {
        return false;
    }
    // Save socket and return true if found
    userSocket = socketPtr->second;
    return true;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                         GET USER NAME FOR THE GIVEN SOCKET
bool TcpService::resolveUserNameBySocket(const std::shared_ptr<QTcpSocket> & userSocket,
                                         QString & userName)
{
    // Try to find given socket
    const auto userNameAndSocket =
            find_if(m_nameToSocket.begin(),
                    m_nameToSocket.end(),
                    [userSocket] (std::pair<std::string,
                                  std::shared_ptr<QTcpSocket>> nameSocket)
                    {
                        return nameSocket.second == userSocket;
                    });
    // Return false if not found
    if(userNameAndSocket == m_nameToSocket.end())
    {
        return false;
    }
    // Save user name and return true if found
    userName = QString(userNameAndSocket->first.c_str());
    return true;
}

