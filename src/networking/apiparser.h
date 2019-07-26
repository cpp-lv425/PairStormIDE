#ifndef APIPARSER_H
#define APIPARSER_H


#include <QObject>
#include "udpservice.h"
#include "tcpservice.h"
#include "tcpudphelpers.h"

class BaseApiParser : public QObject
{
    Q_OBJECT

protected:
    std::shared_ptr<UdpService> m_udpService;
    //std::shared_ptr<TcpService> m_tcpService;
    //ServerData                  m_serverData;
    bool                        m_isServerDataConfigured;

    explicit BaseApiParser(QObject *qObject = nullptr);
public:

    // Also copying is restricted for singletons
    BaseApiParser(BaseApiParser const&) = delete;
    BaseApiParser& operator=(BaseApiParser const&) = delete;

    // Singleton instance generator
    static std::shared_ptr<UdpService> getInstance();

    virtual void testSendDatagram() = 0;
    virtual void configureServerData() = 0;
    virtual void shareServerData() = 0;

signals:
    //virtual void messageReceived() = 0;
    //virtual void changeReceived() = 0;

public slots:
    virtual void shareServerDataOnLogin() = 0;
    virtual void processUdpDatagramOnReceive() = 0;
};

class ApiParser : public BaseApiParser
{
    Q_OBJECT

    ApiParser();

public:

    // Also copying is restricted for singletons
    ApiParser(ApiParser const&) = delete;
    ApiParser& operator=(ApiParser const&) = delete;

    // Singleton instance generator
    static std::shared_ptr<ApiParser> getInstance();

    virtual void testSendDatagram() override;

    virtual void configureServerData() override;
    virtual void shareServerData() override;

signals:
    //void messageReceived() override;
    //void changeReceived() override;

public slots:
    virtual void shareServerDataOnLogin() override;
    virtual void processUdpDatagramOnReceive() override;
};

#endif // APIPARSER_H
