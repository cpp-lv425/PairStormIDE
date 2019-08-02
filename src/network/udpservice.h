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
//                                                                       UDP SERVICE PROVIDER
// ==========================================================================================
class UdpService : public QObject
{
    Q_OBJECT // for signals and slots


    const PortNumType     mcSocketPortNumber;
    QVector<QHostAddress> mBroadcastAddresses;
    Datagram              mPendingDatagram;

    // QUdpSocket composition
    std::unique_ptr<QUdpSocket> mpUdpSocket;

    void configureSocket();

public:

    explicit UdpService(QObject *qObject = nullptr);
    UdpService(UdpService const&) = delete;
    UdpService& operator=(UdpService const&) = delete;

    void broadcastDatagram(const QString & datagram);

    Datagram getReceivedDatagram() const;

    ~UdpService();

signals:

    void newDatagramSaved();

private slots:

    void saveDatagramOnReceival();
};

#endif // UPDSERVICE_H
