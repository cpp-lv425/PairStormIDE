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

public slots:

    //======================= Slots for GUI events ===========================
    virtual void configureServiceOnLogin(const QString & userName) = 0;

    virtual void startSharing(const QString & userName,
                              const bool isSelfInitiated = true) = 0;
    virtual void stopSharing (const QString & userName,
                              const bool isSelfInitiated = true) = 0;

    //======================= Slots for Editor & Chat events =================
    virtual void shareMessage(const QString & messageContent) = 0;

signals:

    //======================= Signals to GUI =================================
    void serviceFailed ();

    void onlineUsersUpdated    (const LocalConnectorInterface * connector);
    void connectedUsersUpdated (const LocalConnectorInterface * connector);

    void startSharingRequested (const QString userName);
    void stopSharingRequested  (const QString userName);

    //======================= Signals to Editor & Chat =======================
    void messageReceived(const QString userName, const QString message);


//Experimental features ======================================================
#ifdef CUSTOM_DEBUG
public:
    virtual void testSendHelloToLastServer() = 0;
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


private:

    void startBroadcastingAttributes();

private slots:

    void shareAttributesOnTimerTick();
    void addServerFromUdpDatagramOnReceive();

    void processTcpSegmentOnReceive();

public slots:
    virtual void configureServiceOnLogin(const QString & userName) override;

    virtual void startSharing(const QString & userName,
                              const bool isSelfInitiated = true) override;
    virtual void stopSharing(const QString & userName,
                             const bool isSelfInitiated = true)  override;

    virtual void shareMessage(const QString & messageContent) override;


//Experimental features ======================================================
#ifdef CUSTOM_DEBUG
public:
    virtual void testSendHelloToLastServer() override;
#endif //CUSTOM_DEBUG
//============================================================================
};

#endif // APIPARSER_H
