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
        this,               &ApiParser::addServerFromUdpDatagramOnReceive,
        Qt::UniqueConnection
    );
    // TODO connect configureServerOnLogin to the login signal
}

std::shared_ptr<ApiParser> ApiParser::getParser()
{
    static std::shared_ptr<ApiParser> instance(new ApiParser);
    return instance;
}

void ApiParser::boradcastServerAttributes()
{
    QString serverData(m_launchedTcpServerAttrib.toJsonQString());
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
    m_launchedTcpServerAttrib = m_tcpService->getServerData();

    connect(
        m_tcpService.get(), &TcpService::newSegmentSaved,
        this,               &ApiParser::processTcpSegmentOnReceive,
        Qt::UniqueConnection
    );
}

void ApiParser::addServerFromUdpDatagramOnReceive()
{
    // Read received datagram
    Datagram data = m_udpService->getReceivedDatagram();
    // Fill information about server from datagram
    ServerData server_info;
    server_info.fromJsonQString(data.m_data);
    // Save information about server
    m_discoveredTcpServerAttribs.push_back(server_info);


    qDebug() << "add server from udp datagram in api parser:";
    qDebug() << "____________________________________________________________";
    qDebug() << data.m_data;
    qDebug() << "____________________________________________________________";
}

void ApiParser::processTcpSegmentOnReceive()
{
    Segment data = m_tcpService->getReceivedSegment();
    // TODO processing


    qDebug() << "tcp segment in api parser:";
    qDebug() << "____________________________________________________________";
    qDebug() << data.m_data;
    qDebug() << "____________________________________________________________";
}




#ifdef CUSTOM_DEBUG
void ApiParser::testBroadcastServerInfoDatagram()
{
    configureServerOnLogin();
    boradcastServerAttributes();
}

void ApiParser::testSendSegmentToLocalHost()
{
    m_tcpService->sendToHost("Hello, Pair Storm is here", QHostAddress::LocalHost);
}
#endif //CUSTOM_DEBUG
