#include "tcpservice.h"
#include <QtNetwork>
#include <QtCore>

TcpService::TcpService(QObject *qObject) : QObject(qObject)
{
    m_pendingSegment = decltype(m_pendingSegment) ();

    // Create QTcpSocket
    m_tcpServerPtr = std::make_unique<QTcpServer>(new QTcpServer(this));

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
    {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope,
                           QLatin1String("cpp-lv425"),
                           QLatin1String("PairStorm"));

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
        m_netSession = new QNetworkSession(configData, this);//std::make_shared<QNetworkSession>(new QNetworkSession(configData, this));
        //connect(m_netSession.get(), &QNetworkSession::opened, this, &TcpService::sessionOpened);
        connect(m_netSession, &QNetworkSession::opened, this, &TcpService::sessionOpened);

        m_netSession->open();
    }
    else
    {
        sessionOpened();
    }

    connect(
        m_tcpServerPtr.get(), &QTcpServer::newConnection,
        this,                 &TcpService::serveConnectionOnAppearance,
        Qt::UniqueConnection);
}

std::shared_ptr<TcpService> TcpService::getService()
{
    static std::shared_ptr<TcpService>
            serviceInstance(new TcpService);
    return serviceInstance;
}

void TcpService::sendToClient(QString data, QHostAddress ip)
{
    // Find client with specified ip address
    auto clientSocket = std::find_if(
                m_clientSocketPtrs.begin(), m_clientSocketPtrs.end(),
                [ip](std::shared_ptr<QTcpSocket> clientSock)
                {
                    return clientSock->peerAddress() == ip;
                });

    // Create & fill TCP segment with data
    QByteArray segment;
    segment.append(data);
    // Send TCP segment towards the client
    (*clientSocket)->write(segment);
}

void TcpService::disconnectClient(QHostAddress ip)
{
    // Find client with specified ip address
    auto clientSocket = std::find_if(
                m_clientSocketPtrs.begin(), m_clientSocketPtrs.end(),
                [ip](std::shared_ptr<QTcpSocket> clientSock)
                {
                    return clientSock->peerAddress() == ip;
                });
    // Disconnect found client from server
    (*clientSocket)->disconnectFromHost();
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

Segment TcpService::getSavedSegment()
{
    return m_pendingSegment;
}

void TcpService::sessionOpened()
{
    // Save the used configuration
    if (m_netSession != nullptr) {
        QNetworkConfiguration configData = m_netSession->configuration();
        QString id = (configData.type() == QNetworkConfiguration::UserChoice) ?
            m_netSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString() :
            configData.identifier();

        QSettings settings(QSettings::UserScope,
                           QLatin1String("cpp-lv425"),
                           QLatin1String("PairStorm"));

        settings.beginGroup(QLatin1String("PSnet"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }
    // Launch TcpServer
    m_tcpServerPtr = std::make_unique<QTcpServer>(new QTcpServer(this));
    if (!m_tcpServerPtr->listen())
    {
        qDebug() << "Failure: server hasn't been started";
        return;
    }

    qDebug() << "TCP server ip addresses: ";
    for (const auto & ip : QNetworkInterface::allAddresses())
        qDebug() << "    ip: " << ip.toString();
    qDebug() << "TCP server port: " << m_tcpServerPtr->serverPort();
}

void TcpService::serveConnectionOnAppearance()
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
}

void TcpService::saveSegmentOnReceival()
{
    QTcpSocket* readSocket = qobject_cast<QTcpSocket*>(sender());

    // Get received data & source IP
    QByteArray   data(readSocket->readAll());
    QHostAddress ip(readSocket->peerAddress());
    PortNumType  port(readSocket->peerPort());
    // Save segment attributes
    m_pendingSegment.m_data = data;
    m_pendingSegment.m_ip   = ip;
    m_pendingSegment.m_port = port;


    emit newSegmentSaved();
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
