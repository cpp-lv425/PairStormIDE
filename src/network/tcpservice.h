#ifndef TCPSERVICE_H
#define TCPSERVICE_H

// Base features and structures for the networking module
#include "networkbase.h"

#include <memory>
#include <QtCore>
#include <QString>
#include <QObject>
#include <QtNetwork>
#include <QTcpServer>
#include <unordered_map>
#include <QNetworkSession>

// ==========================================================================================
//                                                                       TCP SERVICE PROVIDER
// ==========================================================================================
class TcpService : public QObject
{
    Q_OBJECT // for signals and slots


    // Name of the launched server and its port
    const QString     m_serverName;
    ServerData        m_serverAttributes;
    const PortNumType m_portNumber;

    // Composition of the Network session & TCP server
    std::unique_ptr<QNetworkSession>     m_netSessionPtr;
    std::unique_ptr<QTcpServer>          m_tcpServerPtr;

    Segment                              m_pendingSegment;

    // Fill in internal attributes
    void saveServerAttributes();

    // Send thorough specified socket
    void sendThroughSocket(const QString & data, QTcpSocket * receiver);

public:

    explicit TcpService(const QString & serverName, QObject *qObject = nullptr);
    TcpService(TcpService const&) = delete;
    TcpService& operator=(TcpService const&) = delete;

    bool isActive() const;

    // Service destructor
    ~TcpService();

    // Server attributes getter
    ServerData getServerAttributes() const;

    // Connect to TCP server with specified attributes
    // and create special socket or disconnect from given socket
    bool sendDataToTcpServer(const QString & data, const ServerData & serverData);

    // Saved segment getter
    Segment getReceivedSegment() const;

/*
    void setUserNameSocketRelation(const std::shared_ptr<QTcpSocket> & userSocket, const QString & userName);
    void removeUserNameSocketRelation(const QString & userName);
    bool resolveSocketByUserName(std::shared_ptr<QTcpSocket> & userSocket, const QString & userName);
    bool resolveUserNameBySocket(const std::shared_ptr<QTcpSocket> & userSocket, QString & userName);
*/
signals:

    void newSegmentSaved();

private slots:

    void removeSocketOnDisconnected();
    void configureServer();
    void configureSocketOnServerConnection();
    void configureSocketOnClientConnection();
    void saveSegmentOnReceival();

};

#endif // TCPSERVICE_H
