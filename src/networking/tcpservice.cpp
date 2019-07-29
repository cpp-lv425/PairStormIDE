#include "tcpservice.h"

// ==========================================================================================
// ==========================================================================================
//                                                                    TCP SERVICE CONSTRUCTOR
TcpService::TcpService(QObject *qObject) : QObject(qObject)
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
        m_netSession = std::make_unique<QNetworkSession>(configData, this);

        // Configure server when session opening succeeds
        connect(
            m_netSession.get(), &QNetworkSession::opened,
            this,               &TcpService::configureServer,
            Qt::UniqueConnection);

        m_netSession->open();
    }
    else
    {
        // Configure server immediately if additional
        // network configuration is not needed
        configureServer();
    }

    // Trigger processing of the connections, requested by clients
    connect(
        m_tcpServerPtr.get(), &QTcpServer::newConnection,
        this,                 &TcpService::processClientConnectionOnRequest,
        Qt::UniqueConnection);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                       CONFIGURE TCP SERVER

void TcpService::configureServer()
{
    // Case, if additional configuration has been previously done
    if (m_netSession)
    {
        QNetworkConfiguration configData = m_netSession->configuration();
        QString configId = (configData.type() == QNetworkConfiguration::UserChoice) ?
            m_netSession->sessionProperty(QString("UserChoiceConfiguration")).toString() :
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
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                   GET INSTANCE OF THE TCP SERVICE PROVIDER
std::shared_ptr<TcpService> TcpService::getService()
{
    static std::shared_ptr<TcpService>
            serviceInstance(new TcpService);
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
    std::for_each(m_clientSocketPtrs.begin(), m_clientSocketPtrs.end(),
                  [this](std::shared_ptr<QTcpSocket> socket)
                  {
                      disconnectSocket(socket);
                  });
    std::for_each(m_serverSocketPtrs.begin(), m_serverSocketPtrs.end(),
                  [this](std::shared_ptr<QTcpSocket> socket)
                  {
                      disconnectSocket(socket);
                  });
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                     CONFIGURE NAME OF THE LOCAL TCP SERVER
void TcpService::setServerName(const QString & name)
{
    m_serverName = name;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                 GET CONFIGURATIONS OF THE LOCAL TCP SERVER
ServerData TcpService::getServerData()
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
//                                                                    DISCONNECT GIVEN SOCKET
void TcpService::disconnectSocket(std::shared_ptr<QTcpSocket> socket)
{
    socket->disconnectFromHost();
    socket->close();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                      GET SOCKET OUT OF THE GIVEN USER NAME
bool TcpService::resolveSocketByUserName(std::shared_ptr<QTcpSocket> & userSocket,
                                         const QString & userName)
{
    const auto socketPtr = m_nameToSocket.find(userName.toStdString());
    if(socketPtr == m_nameToSocket.end())
    {
        return false;
    }
    userSocket = socketPtr->second;
    //userSocket = m_nameToSocket[userName.toStdString()];
    return true;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                      GET SOCKET OUT OF THE GIVEN USER NAME
void TcpService::connectToTcpServer(const ServerData & serverData)// QHostAddress ip, PortNumType port)
{
    // TODO: establish function
    // Create new empty server socket & save its ptr
    std::shared_ptr<QTcpSocket>
            serverSocketPtr(new QTcpSocket(this));
    m_serverSocketPtrs.push_back(serverSocketPtr);

    // Trigger processing in case of successful connection
    connect(
        serverSocketPtr.get(), &QTcpSocket::connected,
        this,                  &TcpService::processServerConnectionOnRequest,
        Qt::AutoConnection);

    // And try to connect
    //serverSocketPtr->connectToHost(ip, port);
}

Segment TcpService::getReceivedSegment()
{
    return m_pendingSegment;
}

void TcpService::processServerConnectionOnRequest()
{
    // Get connected host' socket ptr
    std::shared_ptr<QTcpSocket>
            serverSocketPtr(qobject_cast<QTcpSocket*>(sender()));

    // Trigger saving received segments on readyRead
    connect(
        serverSocketPtr.get(), &QTcpSocket::readyRead,
        this,                  &TcpService::saveSegmentOnReceival,
        Qt::AutoConnection);

    // Trigger informing about disconnected hosts
    connect(
        serverSocketPtr.get(), &QTcpSocket::disconnected,
        this,                  &TcpService::informAboutStatusOnDisconnected,
        Qt::AutoConnection);


    // TODO establish normal connection


    // Send hello message to the server
    sendThroughSocket(
        "Hello, my server, I am connected to you now",
        serverSocketPtr);
}

void TcpService::processClientConnectionOnRequest()
{
    // Get brand new client' socket and save its ptr
    std::shared_ptr<QTcpSocket>
            clientSocketPtr(m_tcpServerPtr->nextPendingConnection());
    m_clientSocketPtrs.push_back(clientSocketPtr);

    // Trigger saving received segments on readyRead
    connect(
        clientSocketPtr.get(), &QTcpSocket::readyRead,
        this,                  &TcpService::saveSegmentOnReceival,
        Qt::AutoConnection);

    // Trigger informing about disconnected hosts
    connect(
        clientSocketPtr.get(), &QTcpSocket::disconnected,
        this,                  &TcpService::informAboutStatusOnDisconnected,
        Qt::AutoConnection);


    // TODO establish normal connection


    // Send hello message to the client
    sendThroughSocket(
        "Hello, my client, you are connected to me now",
        clientSocketPtr);
}

void TcpService::informAboutStatusOnDisconnected()
{
    // Get disconnected socket
    std::shared_ptr<QTcpSocket>
            disconnectedSocketPtr(qobject_cast<QTcpSocket*>(sender()));

    qDebug() << "Host has been disconnected:";
    qDebug() << "    ip   -> " << disconnectedSocketPtr->peerAddress().toString();
    qDebug() << "    port -> " << disconnectedSocketPtr->peerPort();

    // TODO emit signal of socket disconnection
}

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



/*
void TcpService::addIpServerNameRelation(QHostAddress ip, QString serverName)
{
    m_ipToServerName[ip.toString().toStdString()] = serverName;
}

QString TcpService::getServerNameByIp(QHostAddress ip)
{
    if(m_ipToServerName.find(ip.toString().toStdString()) == m_ipToServerName.end())
    {
        return QString();
    }
    return m_ipToServerName[ip.toString().toStdString()];
}
*/
/*

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

Server::Server(QWidget *parent)
    : QDialog(parent)
{

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::sendFortune);
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        qDebug() << "error while listening";
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    qDebug() << "server is running with ip: " << ipAddress;
    qDebug() << "port of the server: " << tcpServer->serverPort();
}

//! [4]
void Server::sendFortune()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << fortunes[QRandomGenerator::global()->bounded(fortunes.size())];

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    QHostAddress addr = clientConnection->peerAddress();

    clientConnection->write(block);
    clientConnection->disconnectFromHost();
}
*/
