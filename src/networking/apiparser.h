#ifndef APIPARSER_H
#define APIPARSER_H


#include <QObject>
#include "udpservice.h"
#include "tcpservice.h"

class BaseApiParser : public QObject
{
    Q_OBJECT

protected:
    std::shared_ptr<UdpService> m_udpService;
    std::shared_ptr<TcpService> m_tcpService;

public:
    explicit BaseApiParser(QObject *qObject = nullptr);

    virtual void testSendDatagram() = 0;

public slots:
    virtual void processUdpDatagramOnReceive() = 0;
};

class ApiParser : public BaseApiParser
{
    Q_OBJECT

public:
    explicit ApiParser();

    virtual void testSendDatagram() override;

signals:

public slots:
    virtual void processUdpDatagramOnReceive() override;
};

#endif // APIPARSER_H
