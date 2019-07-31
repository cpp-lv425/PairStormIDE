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

    // Attributes of discovered TCP servers
    QVector<ServerData> m_discoveredTcpServersAttrib;

    // Attributes of connected TCP servers
    QVector<ServerData> m_connectedTcpServersAttrib;

public:

    LocalConnectorInterface(LocalConnectorInterface const&)            = delete;
    LocalConnectorInterface& operator=(LocalConnectorInterface const&) = delete;


    virtual QVector<QString> getOnlineUsers() const = 0;
    virtual QVector<QString> getConnectedUsers() const = 0;
    virtual void requestSharingWithUser(QString userName)    = 0;

signals:

    //========================== To the  GUI ==========================
    void serviceFailed();
    void sharingRequested(QString userName);
    void newUserDiscovered();

    //======================= To  Editor & Chat =======================
    void messageReceived();
    void changeReceived();


private slots:
    //====================== On internal signals ======================

    // When either UDP datagram or TCP segment has been received
    virtual void addServerFromUdpDatagramOnReceive() = 0;
    virtual void processTcpSegmentOnReceive()        = 0;

    virtual void processConnectionOnRequest(QTcpSocket * clientSocketPtr)        = 0;

    // When documents synchronization is needed between users
    //virtual void requireFullTextDocumentOnNeed();

    virtual void broadcastServerAttributesOnTimerTick() = 0;

    //====================== On external signals ======================

public slots:
    // When the uer has recently logged in to the application
    virtual void configureServiceOnLogin(const QString & userName) = 0;

    // Each second when the counter emits signal


    // When editor emits change or chat emits message
    //virtual void informAttachedHostsAboutChanges() = 0;
    //virtual void sendMessageToAttachedHosts()      = 0;


#ifdef CUSTOM_DEBUG
public:
    virtual void testConnectToValik()      = 0;
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
    virtual QVector<QString> getConnectedUsers() const override;
    virtual void requestSharingWithUser(QString userName) override;


signals:
    // All signals are in the base class

public slots:

    virtual void addServerFromUdpDatagramOnReceive() override;
    virtual void processTcpSegmentOnReceive() override;

    virtual void broadcastServerAttributesOnTimerTick() override;

    //virtual void requireFullTextDocumentOnNeed() override;

    virtual void processConnectionOnRequest(QTcpSocket * clientSocketPtr) override;


    virtual void configureServiceOnLogin(const QString & userName) override;


    //virtual void informAttachedHostsAboutChanges() override;
    //virtual void sendMessageToAttachedHosts() override;



#ifdef CUSTOM_DEBUG
public:
    virtual void testConnectToValik() override;
#endif //CUSTOM_DEBUG
};

#endif // APIPARSER_H
