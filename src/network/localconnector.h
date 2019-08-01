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

    virtual void startSharingOnCommand(const QString userName) = 0;
    virtual void stopSharingOnCommand (const QString userName) = 0;

    //======================= Slots for Editor & Chat events =================
    virtual void shareMessage(const QString messageContent) = 0;

signals:

    //======================= Signals to GUI =================================
    void serviceFailed ();

    void onlineUsersUpdated    (const QStringList users);
    void connectedUsersUpdated (const QStringList users);

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
    // Timer for regular clearing outdated attributes
    std::unique_ptr<QTimer> m_internalClearOutdatedTimer;

public:

    DefaultLocalConnector() { }
    DefaultLocalConnector(DefaultLocalConnector const&)            = delete;
    DefaultLocalConnector& operator=(DefaultLocalConnector const&) = delete;


    virtual QStringList getOnlineUsers()    const override;
    virtual QStringList getConnectedUsers() const override;


private:

    void startBroadcastingAttributes();
    void startClearingOutdatedAttributes();
    void startSharing(const QString userName,
                      const bool isSelfInitiated = true);
    void stopSharing(const QString userName,
                     const bool isSelfInitiated = true);

private slots:

    virtual void startSharingOnCommand(const QString userName) override;
    virtual void stopSharingOnCommand (const QString userName) override;

    void shareAttributesOnTimerTick();
    void clearOutdatedAttributesOnTimerTick();
    void addServerFromUdpDatagramOnReceive();

    void processTcpSegmentOnReceive();

public slots:
    virtual void configureServiceOnLogin(const QString & userName) override;


    virtual void shareMessage(const QString messageContent) override;


//Experimental features ======================================================
#ifdef CUSTOM_DEBUG
public:
    virtual void testSendHelloToLastServer() override;
#endif //CUSTOM_DEBUG
//============================================================================
};

#endif // APIPARSER_H
