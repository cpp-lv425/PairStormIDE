#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <memory>
#include <unordered_map>
#include <QString>
#include <QObject>
#include <QTcpServer>
#include <QNetworkSession>
#include "networkbasestructures.h"

// SINGLETON
// TCP service provider
class TcpService : public QObject
{
    Q_OBJECT // for signals and slots


    // Network session if needed
    // Composition of the TCP server
    // Set of the connected TCP
    QNetworkSession*                    m_netSession = nullptr;
    //std::shared_ptr<QNetworkSession>     m_netSession;
    std::unique_ptr<QTcpServer>          m_tcpServerPtr;
    QVector<std::shared_ptr<QTcpSocket>> m_clientSocketPtrs;


    // Standard port number for TCP communication
    // Received segments using the port m_portNumber
    const PortNumType m_portNumber = g_defaultTcpPortNumber;
    Segment m_pendingSegment;

    QString m_serverName;

    std::unordered_map<std::string, QString> m_ipToServerName;

    explicit TcpService(QObject *qObject = nullptr);

public:

    TcpService(TcpService const&) = delete;
    TcpService& operator=(TcpService const&) = delete;

    // Service getter
    static std::shared_ptr<TcpService> getService();

    ServerData getServerData();

    void sendToHost(QString data, QHostAddress ip);
    void disconnectClient(QHostAddress ip);

    void addIpServerNameRelation(QHostAddress ip, QString serverName);
    QString getServerNameByIp(QHostAddress ip);



    void giveNameToServer(QString name);

    void connectToTcpServer();
    Segment getReceivedSegment();

signals:
    void newSegmentSaved();

public slots:
    void sessionOpened();
    void serveConnectionOnAppearance();
    void saveSegmentOnReceival();

};

/*
#include <QDialog>
#include <QString>
#include <QVector>


QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);

private slots:
    void sessionOpened();
    void sendFortune();

private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    QNetworkSession *networkSession = nullptr;
};
//! [0]
//!
*/
#endif // TCPSERVICE_H
