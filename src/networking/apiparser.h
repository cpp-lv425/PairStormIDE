#ifndef APIPARSER_H
#define APIPARSER_H


#include <QObject>
#include "udpservice.h"
#include "tcpservice.h"
#include "networkbasestructures.h"

class BaseApiParser : public QObject
{
    Q_OBJECT

protected:
    std::shared_ptr<UdpService> m_udpService;
    std::shared_ptr<TcpService> m_tcpService;
    QString                     m_userName;

    ServerData                  m_tcpServerDataThis;
    QVector<QString>            m_tcpServerDataNeighbors;

    //bool                        m_isServerDataConfigured;

    explicit BaseApiParser(QObject *qObject = nullptr);
public:

    // Also copying is restricted for singletons
    BaseApiParser(BaseApiParser const&) = delete;
    BaseApiParser& operator=(BaseApiParser const&) = delete;

    // Singleton instance generator
    static std::shared_ptr<UdpService> getInstance();

    virtual void testSendDatagram() = 0;
    virtual void testSendSegmentToUser(const QString & userName) = 0;
    virtual void setUserName(const QString & userName) = 0;

signals:
    //virtual void messageReceived() = 0;v
    //virtual void changeReceived() = 0;

public slots:
    virtual void configureServerOnLogin() = 0;
    virtual void shareServerData() = 0;
    virtual void processUdpDatagramOnReceive() = 0;
    virtual void processTcpSegmentOnReceive() = 0;
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

    virtual void setUserName(const QString & userName) override;

    virtual void testSendDatagram() override;
    virtual void testSendSegmentToUser(const QString & userName) override;

signals:
    //void messageReceived() override;
    //void changeReceived() override;

public slots:
    virtual void configureServerOnLogin() override;
    virtual void shareServerData() override;
    virtual void processUdpDatagramOnReceive() override;
    virtual void processTcpSegmentOnReceive() override;
};

#endif // APIPARSER_H
