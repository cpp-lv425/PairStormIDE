#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H


#include <QObject>

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    ConnectionManager(QObject *parent = nullptr);
    bool hasConnection();
private:
};

#endif // CONNECTIONMANAGER_H
