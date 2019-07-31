#ifndef APIPARSER_H
#define APIPARSER_H

#include "networkbase.h"
#include "tcpservice.h"
#include "udpservice.h"

// ==========================================================================================
//                                                                        CONNECTOR INTERFACE
class LocalConnectorInterface : public QObject
{
    Q_OBJECT

protected:

    explicit LocalConnectorInterface(QObject *qObject = nullptr) :
        QObject(qObject) { }

public:

    virtual QStringList getOnlineUsers()    const = 0;
    virtual QStringList getConnectedUsers() const = 0;

    virtual void startSharing(const QString & userName) = 0;

public slots:

    //======================= Slots for GUI events ===========================
    virtual void configureServiceOnLogin(const QString & userName) = 0;

    //======================= Slots for Editor & Chat events =================
    //virtual void informAttachedHostsAboutChanges() = 0;
    //virtual void sendMessageToAttachedHosts()      = 0;

signals:

    //======================= Signals to GUI =================================
    void serviceFailed();
    void newUserDiscovered();
    void sharingRequested(QString userName);

    //======================= Signals to Editor & Chat =======================
    void changeReceived();
    void messageReceived();


//Experimental features ======================================================
#ifdef CUSTOM_DEBUG
public:
    virtual void testConnectToValik() = 0;
#endif //CUSTOM_DEBUG
//============================================================================
};


// ==========================================================================================
//                                                                          DEFAULT CONNECTOR
class DefaultLocalConnector : public LocalConnectorInterface
{
    Q_OBJECT

private:

    // UDP & TCP service providers
    std::shared_ptr<UdpService> m_udpService;
    std::shared_ptr<TcpService> m_tcpService;

    // Attributes of discovered & connected TCP servers
    QVector<ServerData> m_discoveredTcpServersAttrib;
    QVector<ServerData> m_connectedTcpServersAttrib;

    // Timer for regular broadcasting
    std::unique_ptr<QTimer> m_internalBroadcastTimer;

public:

    DefaultLocalConnector() { }
    DefaultLocalConnector(DefaultLocalConnector const&)            = delete;
    DefaultLocalConnector& operator=(DefaultLocalConnector const&) = delete;


    virtual QStringList getOnlineUsers()    const override;
    virtual QStringList getConnectedUsers() const override;

    virtual void startSharing(const QString & userName) override;

private:

    void startBroadcastingAttributes();

private slots:

    void addServerFromUdpDatagramOnReceive();
    void processTcpSegmentOnReceive();

    void broadcastServerAttributesOnTimerTick();

    //virtual void requireFullTextDocumentOnNeed() override;

    void processConnectionOnRequest(QTcpSocket * clientSocketPtr);

public slots:

    virtual void configureServiceOnLogin(const QString & userName) override;


//Experimental features ======================================================
#ifdef CUSTOM_DEBUG
public:
    virtual void testConnectToValik() override;
#endif //CUSTOM_DEBUG
//============================================================================
};

#endif // APIPARSER_H
