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
        Qt::UniqueConnection);

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
    auto usersServerData =
            std::find_if(m_discoveredTcpServersAttrib.begin(),
                         m_discoveredTcpServersAttrib.end(),
                         [userName] (const ServerData & serverData)
                         {
                             return serverData.m_name == userName;
                         });
    // Return if not found any server with a given userName
    if (usersServerData == m_discoveredTcpServersAttrib.end()) return;
    // Else try to establish connection with server
    m_tcpService->connectToTcpServer(*usersServerData);
}

void ApiParser::boradcastServerAttributes()
{
    // Push attributes to the Json bearer string & broadcast it
    QString serverData(m_launchedTcpServerAttrib.toJsonQString());
    m_udpService->broadcastDatagram(serverData);
}

void ApiParser::configureServerOnLogin(const QString & userName)
{
    // Create TCP server
    m_tcpService = TcpService::getService();

    // Define server name with name of the user
    // and save server informtation
    m_tcpService->setServerName(userName);
    m_launchedTcpServerAttrib = m_tcpService->getServerData();



    connect(
        m_tcpService.get(), &TcpService::clientRequestConnection,
        this,               &ApiParser::processConnectionOnRequest,
        Qt::UniqueConnection);

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
    ServerData serverData;
    serverData.fromJsonQString(data.m_data);



#ifdef CUSTOM_DEBUG
    qDebug() << "____________________________________________________________";
    qDebug() << "SERVER IS DISCOVERED THROUGH UDP DISCOVERY PROTOCOL:";
    qDebug() << "name ->" << serverData.m_name;
    qDebug() << "port ->" << serverData.m_port;
    for(const auto & ip : serverData.m_ips)
        qDebug() << "ip -> " << ip.toString();
    qDebug() << "____________________________________________________________";
#endif // CUSTOM_DEBUG



    // If datagram was corrupted
    if (serverData.empty()) return;

    // If discovered server has the name of the current user
    if (serverData.m_name == m_userName) return;

    // If datagram has already been saved
    auto attribPtr = std::find_if(m_discoveredTcpServersAttrib.cbegin(),
                                  m_discoveredTcpServersAttrib.cend(),
                                  [serverData] (const ServerData & inServerData)
    {
        return inServerData.m_name != serverData.m_name;
    });
    if (attribPtr != m_discoveredTcpServersAttrib.cend()) return;

    // Finally, save information about server
    m_discoveredTcpServersAttrib.push_back(serverData);
}



void ApiParser::processTcpSegmentOnReceive()
{
    Segment data = m_tcpService->getReceivedSegment();


    // TODO processing




#ifdef CUSTOM_DEBUG
    qDebug() << "____________________________________________________________";
    qDebug() << "TCP SEGMENT HAS BEEN RECEIVED: ";
    qDebug() << "data ->" << data.m_data;
    qDebug() << "ip -> " << data.m_ip.toString();
    qDebug() << "port ->" << data.m_port;
    qDebug() << "____________________________________________________________";
#endif // CUSTOM_DEBUG
}

void ApiParser::processConnectionOnRequest(std::shared_ptr<QTcpSocket> clientSocketPtr)
{
    // TODO process connection

    QString userName;

    emit sharingRequested(userName);
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
