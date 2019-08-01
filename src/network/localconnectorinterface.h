#ifndef LOCALCONNECTORINTERFACE_H
#define LOCALCONNECTORINTERFACE_H

#include <QObject>

// ==========================================================================================
//                                                                        CONNECTOR INTERFACE
class LocalConnectorInterface : public QObject
{
    Q_OBJECT

protected:

    explicit LocalConnectorInterface(QObject *qObject = nullptr) :
        QObject(qObject) { }

public:

    virtual QStringList getOnlineUsers()    const = 0;
    virtual QStringList getConnectedUsers() const = 0;

public slots:

    //======================= Slots for GUI events ===========================
    virtual void configureOnLogin(const QString & userName) = 0;

    virtual void startSharing(const QString userName) = 0;
    virtual void stopSharing (const QString userName) = 0;

    //======================= Slots for Editor & Chat events =================
    virtual void shareMessage(const QString messageContent) = 0;
    virtual void shareChange(const QString changeContent)   = 0;

signals:

    //======================= Signals to GUI =================================
    void serviceStatus (bool status);

    void onlineUsersUpdated    (const QStringList users);
    void connectedUsersUpdated (const QStringList users);

    void startSharingRequested (const QString userName);
    void stopSharingRequested  (const QString userName);

    //======================= Signals to Editor & Chat =======================
    void messageReceived(const QString userName, const QString message);
};

#endif // LOCALCONNECTORINTERFACE_H
