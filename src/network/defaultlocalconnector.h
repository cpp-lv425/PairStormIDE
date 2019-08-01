#ifndef APIPARSER_H
#define APIPARSER_H

#include "localconnectorinterface.h"
#include "networkbase.h"
#include "tcpservice.h"
#include "udpservice.h"

// ==========================================================================================
//                                                                          DEFAULT CONNECTOR
class DefaultLocalConnector : public LocalConnectorInterface
{
    Q_OBJECT

private:

    // UDP & TCP service providers
    std::shared_ptr<UdpService> mpUdpService;
    std::shared_ptr<TcpService> mpTcpService;

    // Attributes of discovered & connected TCP servers
    QVector<ServerData> mDiscoveredServersAttrib;
    QVector<ServerData> mConnectedServersAttrib;

    // Timer for regular broadcasting & clearing outdated attributes
    std::unique_ptr<QTimer> mpBroadcastingTimer;
    std::unique_ptr<QTimer> mAttributesObsoletionTimer;

public:

    DefaultLocalConnector() { }
    DefaultLocalConnector(DefaultLocalConnector const&)            = delete;
    DefaultLocalConnector& operator=(DefaultLocalConnector const&) = delete;


    virtual QStringList getOnlineUsers()    const override;
    virtual QStringList getConnectedUsers() const override;


private:

    void startSharingAttributes();
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
    void addServerAttributesOnReceive();

    void processTcpSegmentOnReceive();

public slots:
    virtual void configureOnLogin(const QString & userName) override;


    virtual void shareMessage(const QString messageContent) override;


//Experimental features ======================================================
#ifdef CUSTOM_DEBUG
public:
    void testSendHelloToLastServer();
#endif //CUSTOM_DEBUG
//============================================================================
};

#endif // APIPARSER_H
