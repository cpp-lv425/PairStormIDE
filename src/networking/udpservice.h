#ifndef UPDSERVICE_H
#define UPDSERVICE_H

// Base features and structures for the networking module
#include "networkbase.h"

#include <memory>
#include <QObject>
#include <QHostInfo>
#include <QUdpSocket>
#include <QNetworkInterface>

// ==========================================================================================
//                                                                                   SNGLETON
//                                                                       UDP service provider
// ==========================================================================================
class UdpService : public QObject
{
    Q_OBJECT // for signals and slots


    // Port number for UDP service
    const PortNumType m_portNumber;

    // QUdpSocket composition
    std::unique_ptr<QUdpSocket> m_udpSocketPtr;
    // Received datagrams and broadcast IPv4 addresses
    Datagram                    m_pendingDatagram;
    QVector<QHostAddress>       m_broadcastIps;

    explicit UdpService(QObject *qObject = nullptr); // Private!

    // Socket configuration routine, have to be executed
    // before using class - is called inside constructor
    void configureSocket();

public:

    UdpService(UdpService const&) = delete;
    UdpService& operator=(UdpService const&) = delete;

    // Service instance generator
    static std::shared_ptr<UdpService> getService();



    // Datagram broadcaster
    void broadcastDatagram(const QString & datagram);

    // Saved datagram getter
    Datagram getReceivedDatagram() const;

signals:

    void newDatagramSaved();

private slots:

    void saveDatagramOnReceival();
};

#endif // UPDSERVICE_H
