#ifndef CONNECTIONSERVICEPROVIDER_H
#define CONNECTIONSERVICEPROVIDER_H

#include <QObject>
#include "udpservice.h"

class ConnectionServiceProvider : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionServiceProvider(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CONNECTIONSERVICEPROVIDER_H
