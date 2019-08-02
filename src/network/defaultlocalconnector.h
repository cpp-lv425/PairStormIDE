#ifndef APIPARSER_H
#define APIPARSER_H

#include "networkbase.h"

#include "localconnectorinterface.h"
#include "tcpservice.h"
#include "udpservice.h"

// ==========================================================================================
//                                                                          DEFAULT CONNECTOR
// ==========================================================================================
class DefaultLocalConnector : public LocalConnectorInterface
{
    Q_OBJECT

public:

    DefaultLocalConnector() { }
    DefaultLocalConnector(DefaultLocalConnector const&)            = delete;
    DefaultLocalConnector& operator=(DefaultLocalConnector const&) = delete;

public slots:

    virtual void configureOnLogin(const QString & userName) override;

    virtual void startSharing(const QString userName)       override;
    virtual void stopSharing (const QString userName)       override;

    virtual void shareMessage(const QString messageContent) override;
    virtual void shareChange (const QString changeContent)  override;

private:

    // UDP & TCP service providers
    std::shared_ptr<UdpService> mpUdpService;
    std::shared_ptr<TcpService> mpTcpService;

    // Attributes of discovered & connected user' servers
    QVector<ServerData> mDiscoveredServersAttrib;
    QVector<ServerData> mConnectedServersAttrib;

    // Timers for regular broadcasting & clearing outdated attributes
    std::unique_ptr<QTimer> mpBroadcastingTimer;
    std::unique_ptr<QTimer> mAttributesObsoletionTimer;

    void startSharingAttributes();
    void startClearingOutdatedAttributes();

    ServerData popFromConnectedAttributes(const QString & serverName);
    ServerData pushToConnectedAttributes (const QString & serverName);

    QStringList getOnlineUsers()    const;
    QStringList getConnectedUsers() const;

private slots:

    void shareAttributesOnTimerTick();
    void clearOutdatedAttributesOnTimerTick();

    void addServerAttributesOnReceive();
    void parseTcpSegmentOnReceive();
};

#endif // APIPARSER_H
