#include "tcpservice.h"
#include <QtNetwork>
#include <QtCore>

TcpService::TcpService(QObject *qObject) : QObject(qObject)
{
    m_pendingSegment = decltype(m_pendingSegment) ();

    // Create QTcpSocket
    m_tcpServerPtr = std::make_unique<QTcpServer>(new QTcpServer(this));

    //=============================================================================================
    //                                                                    TODO: REFACTOR THIS STUFF
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
    {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, g_orgLabel, g_appLabel);

        settings.beginGroup(QLatin1String("PSnet"));
        const QString configId = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration configData = manager.configurationFromIdentifier(configId);
        if ((configData.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered)
        {
            configData = manager.defaultConfiguration();
        }

        // Establish session
        m_netSession = new QNetworkSession(configData, this);
        //std::make_shared<QNetworkSession>(new QNetworkSession(configData, this));
        //connect(m_netSession.get(), &QNetworkSession::opened, this, &TcpService::sessionOpened);
        connect(m_netSession, &QNetworkSession::opened, this, &TcpService::sessionOpened);

        m_netSession->open();
    }
    else
    {
        sessionOpened();
    }
    //=============================================================================================

    // Trigger processing of the connections, requested by clients
    connect(
        m_tcpServerPtr.get(), &QTcpServer::newConnection,
        this,                 &TcpService::processClientConnectionOnRequest,
        Qt::UniqueConnection);
}

TcpService::~TcpService()
{
    // Close all opened sockets
    std::for_each(m_clientSocketPtrs.begin(), m_clientSocketPtrs.end(),
        [](std::shared_ptr<QTcpSocket> socket)
        {
            socket->close();
        });
    std::for_each(m_serverSocketPtrs.begin(), m_serverSocketPtrs.end(),
        [](std::shared_ptr<QTcpSocket> socket)
        {
            socket->close();
        });
    m_tcpServerPtr->close();
}

void TcpService::sessionOpened()
{
    //=============================================================================================
    //                                                                    TODO: REFACTOR THIS STUFF
    // Save the used configuration
    if (m_netSession != nullptr)
    {
        QNetworkConfiguration configData = m_netSession->configuration();
        QString id = (configData.type() == QNetworkConfiguration::UserChoice) ?
            m_netSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString() :
            configData.identifier();

        QSettings settings(QSettings::UserScope, g_orgLabel, g_appLabel);

        settings.beginGroup(QLatin1String("PSnet"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }
    //=============================================================================================

    // Launch the only QTcpServer instance on the m_portNumber port
    m_tcpServerPtr = std::make_unique<QTcpServer>(new QTcpServer(this));
    if (!m_tcpServerPtr->listen(QHostAddress::Any, m_portNumber))
    {
        qDebug() << "Failure: server hasn't been started";
        m_tcpServerPtr->close();
        return;
    }
    qDebug() << "Success: server has been started on port: " << m_tcpServerPtr->serverPort();
}

std::shared_ptr<TcpService> TcpService::getService()
{
    static std::shared_ptr<TcpService>
            serviceInstance(new TcpService);
    return serviceInstance;
}

ServerData TcpService::getServerData()
{
    ServerData serverData;

    serverData.m_name = m_serverName;
    /*
    for (const auto & ip : QNetworkInterface::allAddresses())
        serverData.m_ips.push_back(ip);
    serverData.m_port = m_tcpServerPtr->serverPort();
    */
    foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags flags = netInterface.flags();
        if( (bool)(flags & QNetworkInterface::IsRunning) && !(bool)(flags & QNetworkInterface::IsLoopBack)){
            foreach (const QNetworkAddressEntry &address, netInterface.addressEntries()) {
                if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    //qDebug() << address.ip().toString();
                    //qDebug() << address.broadcast().toString();
                    serverData.m_ips.push_back(address.ip());
            }
        }
    }
    serverData.m_port = m_tcpServerPtr->serverPort();
    return serverData;
}

void TcpService::sendThroughSocket(const QString & data, std::shared_ptr<QTcpSocket> receiver)
{
    receiver->write(data.toUtf8());
    receiver->flush();
    /*
    // Find client with specified ip address & return if failed
    auto clientSocket = std::find_if(
                m_clientSocketPtrs.begin(),
                m_clientSocketPtrs.end(),
                [ip] (std::shared_ptr<QTcpSocket> clientSock)
                {
                    return clientSock->peerAddress() == ip;
                });
    if (clientSocket == m_clientSocketPtrs.end())
    {
        return;
    }
    // Create & fill TCP segment with data
    QByteArray segment;
    segment.append(data);
    // Send TCP segment towards the client
    (*clientSocket)->write(segment);
    */
}

void TcpService::disconnectSocket(std::shared_ptr<QTcpSocket> socket)
{
    socket->disconnectFromHost();
    /*
    // Find client with specified ip address
    auto clientSocket = std::find_if(
                m_clientSocketPtrs.begin(), m_clientSocketPtrs.end(),
                [ip](std::shared_ptr<QTcpSocket> clientSock)
                {
                    return clientSock->peerAddress() == ip;
                });
    // Disconnect found client from server
    (*clientSocket)->disconnectFromHost();
    */
}

void TcpService::giveNameToLocalServer(QString name)
{
    m_serverName = name;
}

void TcpService::connectToTcpServer(QHostAddress ip, PortNumType port)
{
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
    serverSocketPtr->connectToHost(ip, port);
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
