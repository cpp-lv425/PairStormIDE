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

QVector<QString> ApiParser::getOnlineUsers() const
{
    QVector<QString> userNames;
    for (const auto & serverAttrib : m_discoveredTcpServersAttrib)
        userNames.push_back(serverAttrib.m_name);
    return userNames;
}

void ApiParser::shareWithUser(QString userName)
{
    return;
}

void ApiParser::boradcastServerAttributes()
{
    // Push attributes to the Json bearer string & broadcast it
    QString serverData(m_launchedTcpServerAttrib.toJsonQString());
    m_udpService->broadcastDatagram(serverData);
}

void ApiParser::configureServerOnLogin(const QString userName)
{
    // Create TCP server
    m_tcpService = TcpService::getService();

    // Define server name with name of the user
    // and save server informtation
    m_tcpService->setServerName(userName);
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
    ServerData serverInfo;
    serverInfo.fromJsonQString(data.m_data);
    // Save information about server
    m_discoveredTcpServersAttrib.push_back(serverInfo);


#ifdef CUSTOM_DEBUG
    qDebug() << "____________________________________________________________";
    qDebug() << "SERVER IS DISCOVERED THROUGH UDP DISCOVERY PROTOCOL:";
    qDebug() << "name ->" << serverInfo.m_name;
    qDebug() << "port ->" << serverInfo.m_port;
    for(const auto & ip : serverInfo.m_ips)
        qDebug() << "ip -> " << ip.toString();
    qDebug() << "____________________________________________________________";
#endif // CUSTOM_DEBUG
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
    configureServerOnLogin("Valik");
    boradcastServerAttributes();
}

void ApiParser::testSendSegmentToLocalHost()
{
    //m_tcpService->sendThroughSocket("Hello, Pair Storm is here", QHostAddress::LocalHost);
}
#endif //CUSTOM_DEBUG
