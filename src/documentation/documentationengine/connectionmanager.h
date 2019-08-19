#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H


#include <QObject>

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    ConnectionManager(QObject *parent = nullptr);
    bool hasConnection();
};

#endif // CONNECTIONMANAGER_H
