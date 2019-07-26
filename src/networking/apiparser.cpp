#include "apiparser.h"

BaseApiParser::BaseApiParser(QObject *qObject) : QObject(qObject)
{
    return;
}

ApiParser::ApiParser() : BaseApiParser()
{
    m_udpService = UdpService::getService();
    //m_tcpService = TcpService::getInstance();
    connect(
        m_udpService.get(), &UdpService::newDatagramSaved,
        this,               &ApiParser::processUdpDatagramOnReceive,
        Qt::UniqueConnection
    );
    // TODO connect to tcp service also
}

std::shared_ptr<ApiParser> ApiParser::getInstance()
{
    static std::shared_ptr<ApiParser> instance(new ApiParser);
    return instance;
}

void ApiParser::testSendDatagram()
{
    m_udpService->broadcastDatagram(QString("hello from program"));
}

void ApiParser::shareServerData()
{
    m_udpService->broadcastDatagram(QString("hello from program"));
}

void ApiParser::configureServerData()
{

}

void ApiParser::shareServerDataOnLogin()
{
    // Broadcast server information to other

    m_udpService->broadcastDatagram(QString("hello from program"));
}

void ApiParser::processUdpDatagramOnReceive()
{
    qDebug() << "udp datagram in api parser:";
    qDebug() << "____________________________________________________________";
   // qDebug() << m_udpService->getReceivedDatagram();
    qDebug() << "____________________________________________________________";
}
