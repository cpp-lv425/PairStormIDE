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
            this,                  &TcpService::configureServer,
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
    saveServerAttributes();


    // Trigger processing of the connections, requested by clients
    connect(
        m_tcpServerPtr.get(), &QTcpServer::newConnection,
        this,                 &TcpService::configureSocketOnClientConnection,
        Qt::UniqueConnection);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                           GET FINAL CONFIGURATIONS OF THE LOCAL TCP SERVER
void TcpService::saveServerAttributes()
{
    // Insert server name and server port
    m_serverAttributes.m_name = m_serverName;
    m_serverAttributes.m_port = m_tcpServerPtr->serverPort();

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
                m_serverAttributes.m_ips.push_back(ipEntry.ip());
            }
        }
    }
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
//                                                            GET LOCAL TCP SERVER ATTRIBUTES
ServerData TcpService::getServerAttributes() const
{
    return m_serverAttributes;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                     TCP SERVICE DESTRUCTOR
TcpService::~TcpService()
{
    // Disconnect active sockets
    std::for_each(m_connectedSockets.begin(), m_connectedSockets.end(),
                  [this](QTcpSocket * socket)
                  {
                      disconnectSocket(socket);
                  });

    // Close server if it has been successfully launched
    if (m_tcpServerPtr->isListening())
    {
        m_tcpServerPtr->close();
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                           CONNECT TO TCP SERVER BY GIVEN SERVER ATTRIBUTES
bool TcpService::connectToTcpServer(const ServerData & serverData)
{
    // Create new empty server socket & save its ptr
    QTcpSocket *
            serverSocketPtr(new QTcpSocket(this));

    // Force preliminary configuration in case of successful connection
    connect(
        serverSocketPtr, &QTcpSocket::connected,
        this,            &TcpService::configureSocketOnServerConnection,
        Qt::AutoConnection);

    // Try to connect using present server' ip addresses
    const PortNumType  port = serverData.m_port;
    const QVector<QHostAddress> ips = serverData.m_ips;
    QHostAddress ip;
    const auto ipPtr = std::find(ips.cbegin(),
                                 ips.cend(),
                                 serverData.m_sourceIp);
    if (ipPtr != ips.cend())
    {
        qDebug() << "one ip is the same as other ip";
        ip = *ipPtr;
    }

    serverSocketPtr->connectToHost(ip, port);
    if (serverSocketPtr->waitForConnected(1000))
    {
        return true;
    }
    return false;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                 CONFIGURE SOCKET WHEN SERVER HAS CONNECTED
void TcpService::configureSocketOnServerConnection()
{
    // Get connected host' socket ptr
    QTcpSocket *
            serverSocketPtr(qobject_cast<QTcpSocket*>(sender()));
    m_connectedSockets.push_back(serverSocketPtr);

    // Trigger saving received segments on readyRead
    connect(
        serverSocketPtr, &QTcpSocket::readyRead,
        this,                  &TcpService::saveSegmentOnReceival,
        Qt::AutoConnection);

    // Trigger informing about disconnected hosts
    connect(
        serverSocketPtr, &QTcpSocket::disconnected,
        this,                  &TcpService::removeSocketOnDisconnected,
        Qt::AutoConnection);


    //===================================================================================
    //                                                                ATTENTION: HARDCODE

    // TODO establish normal connection
    // TODO write userName -> socket relation

    QString data("Client: hello, I'm now connected");
    sendThroughSocket(data, serverSocketPtr);

    //===================================================================================
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                 CONFIGURE SOCKET WHEN CLIENT HAS CONNECTED
void TcpService::configureSocketOnClientConnection()
{
    // Get brand new client' socket and save its ptr
    QTcpSocket *
            clientSocketPtr(m_tcpServerPtr->nextPendingConnection());
    m_connectedSockets.push_back(clientSocketPtr);

    // Trigger saving received segments on readyRead
    connect(
        clientSocketPtr, &QTcpSocket::readyRead,
        this,                  &TcpService::saveSegmentOnReceival,
        Qt::AutoConnection);

    // Trigger informing about disconnected hosts
    connect(
        clientSocketPtr, &QTcpSocket::disconnected,
        this,                  &TcpService::removeSocketOnDisconnected,
        Qt::AutoConnection);


    //===================================================================================
    //                                                                ATTENTION: HARDCODE

    //emit socketConnected(clientSocketPtr);

    // TODO establish normal connection
    // TODO write userName -> socket relation

    QString data("Server: hello, I'm now connected");
    sendThroughSocket(data, clientSocketPtr);

    //===================================================================================
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                             SEND DATA THROUGH GIVEN SOCKET
void TcpService::sendThroughSocket(const QString & data,
                                   QTcpSocket * receiver)
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
//                                                           RETURNS TRUE IF SERVER IS ACTIVE
bool TcpService::isActive() const
{
    return m_tcpServerPtr->isListening();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                    DISCONNECT GIVEN SOCKET
void TcpService::disconnectSocket(QTcpSocket * socket)
{
    socket->abort();
    socket->disconnectFromHost();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                             REMOVE DISCONNECTED SOCKET FROM ACTIVE SOCKETS
void TcpService::removeSocketOnDisconnected()
{
    qDebug() << "try to deleting socket";
    // Get disconnected socket
    QTcpSocket *
            disconnectedSocketPtr(qobject_cast<QTcpSocket*>(sender()));

    // Suppose that disconnected socket name is unknown
    QString disconnectedSocketName("Unknown");
    // Try to get real name of the server, described by the given socket
    //bool isNameResolved =
    //        resolveUserNameBySocket(disconnectedSocketPtr, disconnectedSocketName);
    //if (isNameResolved)
    //{
    //    removeUserNameSocketRelation(disconnectedSocketName);
    //}

    disconnectedSocketPtr->close();
    disconnectSocket(disconnectedSocketPtr);
    //m_connectedSockets.erase(std::find(m_connectedSockets.begin(), m_connectedSockets.end(), disconnectedSocketPtr));
    /*
    // Move socket away from active sockets
    auto newConnectedSocketsEnd =
            std::remove(m_connectedSockets.begin(),
                        m_connectedSockets.end(),
                        disconnectedSocketPtr);

    // Close the socket
    std::for_each(newConnectedSocketsEnd,
                  m_connectedSockets.end(),
                  [] (QTcpSocket * socket)
                  {
                      socket->close();
                  });

    // Erase socket from active sockets
    m_connectedSockets.erase(newConnectedSocketsEnd);
    */


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
/*
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
*/
