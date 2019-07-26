#ifndef APIPARSER_H
#define APIPARSER_H


#include <QObject>
#include "udpservice.h"
#include "tcpservice.h"
#include "networkbase.h"


//======================================================================================
//                                                                              SNGLETON
//                                                                             INTERFACE
//                                                                        API interfacer
//======================================================================================
class BaseApiParser : public QObject
{
    Q_OBJECT

protected:

    explicit BaseApiParser(QObject *qObject = nullptr); // Protected!

    // Name of the user that has logged in
    QString m_userName;

    // UDP & TCP service providers
    std::shared_ptr<UdpService> m_udpService;
    std::shared_ptr<TcpService> m_tcpService;

    // Attributes of the launched TCP server & discovered TCP servers
    ServerData          m_launchedTcpServerAttrib;
    QVector<ServerData> m_discoveredTcpServerAttribs;

public:

    BaseApiParser(BaseApiParser const&)            = delete;
    BaseApiParser& operator=(BaseApiParser const&) = delete;

    virtual QVector<QString> getOnlineUsers() const = 0;
    virtual void shareWithUser(QString userName) = 0;

signals:
    virtual void sharingRequested();
    virtual void messageReceived();
    virtual void changeReceived();

public slots:
    //====================== On internal signals ======================

    // When either UDP datagram or TCP segment has been received
    virtual void addServerFromUdpDatagramOnReceive() = 0;
    virtual void processTcpSegmentOnReceive()        = 0;

    // When documents synchronization is needed between users
    virtual void requireFullTextDocumentOnNeed();

    //====================== On external signals ======================

    // When the uer has recently logged in to the application
    virtual void configureServerOnLogin() = 0;

    // Each second when the counter emits signal
    virtual void boradcastServerAttributes() = 0;

    // When editor emits change or chat emits message
    virtual void informAttachedHostsAboutChanges() = 0;
    virtual void sendMessageToAttachedHosts()      = 0;
};





//======================================================================================
//                                                                              SNGLETON
//                                                                        API interfacer
//======================================================================================
class ApiParser : public BaseApiParser
{
    Q_OBJECT

private:

    ApiParser(); // Private!

public:

    ApiParser(ApiParser const&)            = delete;
    ApiParser& operator=(ApiParser const&) = delete;

    // The only way to get API parser
    static std::shared_ptr<ApiParser> getParser();


    virtual QVector<QString> getOnlineUsers() const override;
    virtual void shareWithUser(QString userName) override;


signals:
    void sharingRequested() override;
    void messageReceived() override;
    void changeReceived() override;

public slots:

    virtual void addServerFromUdpDatagramOnReceive() override;
    virtual void processTcpSegmentOnReceive() override;

    virtual void requireFullTextDocumentOnNeed() override;


    virtual void configureServerOnLogin() override;

    virtual void boradcastServerAttributes() override;

    virtual void informAttachedHostsAboutChanges() override;
    virtual void sendMessageToAttachedHosts() override;



#ifdef CUSTOM_DEBUG
public:
    virtual void testBroadcastServerInfoDatagram();
    virtual void testSendSegmentToLocalHost();
#endif //CUSTOM_DEBUG
};

#endif // APIPARSER_H
