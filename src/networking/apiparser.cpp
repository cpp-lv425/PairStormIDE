#include "apiparser.h"

BaseApiParser::BaseApiParser(QObject *qObject) : QObject(qObject)
{
    return;
}

ApiParser::ApiParser() : BaseApiParser()
{
    m_udpService = UdpService::getInstance();
    m_tcpService = TcpService::getInstance();
    connect(
        m_udpService.get(), &UdpService::processReceivedDatagram,
        this,               &ApiParser::processUdpDatagramOnReceive,
        Qt::UniqueConnection
    );
    // TODO connect to tcp service also
}

void ApiParser::testSendDatagram()
{
    m_udpService->broadcastBeaconDatagram();
}

void ApiParser::processUdpDatagramOnReceive()
{
    qDebug() << "udp datagram in api parser:";
    qDebug() << "____________________________________________________________";
    qDebug() << m_udpService->getReceivedDatagram();
    qDebug() << "____________________________________________________________";
}
