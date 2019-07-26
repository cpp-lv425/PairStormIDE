#include "apiparser.h"

BaseApiParser::BaseApiParser(QObject *qObject) : QObject(qObject)
{
    return;
}

ApiParser::ApiParser() : BaseApiParser()
{
    m_udpService = UdpService::getService();


    connect(
        m_udpService.get(), &UdpService::newDatagramSaved,
        this,               &ApiParser::processUdpDatagramOnReceive,
        Qt::UniqueConnection
    );
    // TODO connect configureServerOnLogin to the login signal
}

std::shared_ptr<ApiParser> ApiParser::getInstance()
{
    static std::shared_ptr<ApiParser> instance(new ApiParser);
    return instance;
}

void ApiParser::setUserName(const QString & userName)
{
    m_userName = userName;
}

void ApiParser::testSendDatagram()
{
    //m_udpService->broadcastDatagram(QString("hello from program"));
    configureServerOnLogin();
    shareServerData();
}

void ApiParser::testSendSegmentToUser(const QString & userName)
{
    QHostAddress ip = QHostAddress::LocalHost; // TODO parse user name to user ip
    m_tcpService->sendToHost("Hello", ip);
}

void ApiParser::shareServerData()
{
    QString serverData(m_tcpServerDataThis.toJsonQString());
    /*
    serverData.append("{name : \"");
    serverData.append(m_tcpServerData.m_name);
    serverData.append("\"; ips : [");
    for (const auto & ip : m_tcpServerData.m_ips)
    {
        serverData.append("\"");
        serverData.append(ip.toString());
        serverData.append("\", ");
    }
    serverData.append("]; port : ");
    serverData.append(m_tcpServerData.m_port);
    serverData.append("}");
    */
    m_udpService->broadcastDatagram(serverData);
}

void ApiParser::configureServerOnLogin()
{
    // Broadcast server information to other
    m_tcpService = TcpService::getService();

    m_userName = "";    // TODO get username from other class
    QString serverName = m_userName;
    m_tcpService->giveNameToServer(serverName);
    m_tcpServerDataThis = m_tcpService->getServerData();

    connect(
        m_tcpService.get(), &TcpService::newSegmentSaved,
        this,               &ApiParser::processTcpSegmentOnReceive,
        Qt::UniqueConnection
    );
}

void ApiParser::processUdpDatagramOnReceive()
{
    qDebug() << "udp datagram in api parser:";
    qDebug() << "____________________________________________________________";
    auto data = m_udpService->getReceivedDatagram();
    ServerData server_info;
    server_info.fromJsonQString(data.m_data);
    qDebug() << data.m_data;
    qDebug() << "____________________________________________________________";
}

void ApiParser::processTcpSegmentOnReceive()
{
    qDebug() << "tcp segment in api parser:";
    qDebug() << "____________________________________________________________";
    auto data = m_tcpService->getReceivedSegment();
    qDebug() << data.m_data;
    qDebug() << "____________________________________________________________";
}
