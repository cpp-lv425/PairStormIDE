#ifndef TCPSERVICE_H
#define TCPSERVICE_H

// Base features and structures for the networking module
#include "networkbase.h"

#include <memory>
#include <QObject>
#include <QtNetwork>
#include <QTcpServer>
#include <QNetworkSession>

// ==========================================================================================
//                                                                       TCP SERVICE PROVIDER
// ==========================================================================================
class TcpService : public QObject
{
    Q_OBJECT // for signals and slots


    const PortNumType mcServerPortNumber;
    const QString     mcServerName;
    ServerData        mServerAttributes;
    Segment           mPendingSegment;

    // Composition of the Network session & TCP server
    std::unique_ptr<QNetworkSession> mpNetSession;
    std::unique_ptr<QTcpServer>      mpTcpServer;

    void saveServerAttributes();

public:

    explicit TcpService(const QString & serverName, QObject *qObject = nullptr);
    TcpService(TcpService const&)            = delete;
    TcpService& operator=(TcpService const&) = delete;

    bool sendDataToTcpServer(const QString & data, const ServerData & serverData);

    bool       isServerActive()      const;
    ServerData getServerAttributes() const;
    Segment    getReceivedSegment()  const;

    ~TcpService();

signals:

    void newSegmentSaved();

private slots:

    void configureServer();

    void saveSegmentOnReceival();

    void attachSocketOnServerConnected();
    void attachSocketOnClientConnected();
    void closeSocketOnDisconnected();
};

#endif // TCPSERVICE_H
