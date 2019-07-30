#ifndef APIPARSER_H
#define APIPARSER_H


#include <QObject>
#include "udpservice.h"
#include "tcpservice.h"
#include "networkbase.h"


// ==========================================================================================
//                                                                                   SNGLETON
//                                                                                  INTERFACE
//                                                                             API interfacer
// ==========================================================================================
class LocalConnectorInterface : public QObject
{
    Q_OBJECT

protected:

    explicit LocalConnectorInterface(QObject *qObject = nullptr); // Protected!

    // UDP & TCP service providers
    std::shared_ptr<UdpService> m_udpService;
    std::shared_ptr<TcpService> m_tcpService;

    std::unique_ptr<QTimer>     m_internalBroadcastTimer;

    // Attributes of the launched TCP server & discovered TCP servers
    QVector<ServerData> m_discoveredTcpServersAttrib;

public:

    LocalConnectorInterface(LocalConnectorInterface const&)            = delete;
    LocalConnectorInterface& operator=(LocalConnectorInterface const&) = delete;


    virtual QVector<QString> getOnlineUsers() const = 0;
    virtual void shareWithUser(QString userName) = 0;

signals:

    //========================== To the  GUI ==========================
    void sharingRequested(QString userName);
    void newUserDiscovered();

    //======================= To  Editor & Chat =======================
    void messageReceived();
    void changeReceived();


public slots:
    //====================== On internal signals ======================

    // When either UDP datagram or TCP segment has been received
    virtual void addServerFromUdpDatagramOnReceive() = 0;
    virtual void processTcpSegmentOnReceive()        = 0;

    virtual void processConnectionOnRequest(std::shared_ptr<QTcpSocket> clientSocketPtr)        = 0;

    // When documents synchronization is needed between users
    //virtual void requireFullTextDocumentOnNeed();

    virtual void broadcastServerAttributesOnTimerTick() = 0;

    //====================== On external signals ======================

    // When the uer has recently logged in to the application
    virtual void configureServerOnLogin(const QString & userName) = 0;

    // Each second when the counter emits signal


    // When editor emits change or chat emits message
    //virtual void informAttachedHostsAboutChanges() = 0;
    //virtual void sendMessageToAttachedHosts()      = 0;


#ifdef CUSTOM_DEBUG
public:
    virtual void testLoginUser(const QString & userName) = 0;
    virtual void testSendSegmentToLocalHost()      = 0;
#endif //CUSTOM_DEBUG
};





// ==========================================================================================
//                                                                                   SNGLETON
//                                                                             API interfacer
// ==========================================================================================
class DefaultLocalConnector : public LocalConnectorInterface
{
    Q_OBJECT

private:

    DefaultLocalConnector(); // Private!

public:

    DefaultLocalConnector(DefaultLocalConnector const&)            = delete;
    DefaultLocalConnector& operator=(DefaultLocalConnector const&) = delete;

    // The only way to get API parser
    static std::shared_ptr<DefaultLocalConnector> generateConnector();


    virtual QVector<QString> getOnlineUsers() const override;
    virtual void shareWithUser(QString userName) override;


signals:
    // All signals are in the base class

public slots:

    virtual void addServerFromUdpDatagramOnReceive() override;
    virtual void processTcpSegmentOnReceive() override;

    virtual void broadcastServerAttributesOnTimerTick() override;

    //virtual void requireFullTextDocumentOnNeed() override;

    virtual void processConnectionOnRequest(std::shared_ptr<QTcpSocket> clientSocketPtr) override;


    virtual void configureServerOnLogin(const QString & userName) override;


    //virtual void informAttachedHostsAboutChanges() override;
    //virtual void sendMessageToAttachedHosts() override;



#ifdef CUSTOM_DEBUG
public:
    virtual void testLoginUser(const QString & userName) override;
    virtual void testSendSegmentToLocalHost() override;
#endif //CUSTOM_DEBUG
};

#endif // APIPARSER_H
