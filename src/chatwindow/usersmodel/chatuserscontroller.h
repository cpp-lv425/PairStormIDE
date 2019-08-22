#ifndef CHATUSERSCONTROLLER_H
#define CHATUSERSCONTROLLER_H

#include <QObject>
#include <QDateTime>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>


struct ChatUser
{
    QString mUserName;
    enum State : int
    {
        DisconnectedUser,
        ConnectedUser,
        OfflineUser
    } mState;
};

class ChatUsersController : public QObject
{
    Q_OBJECT

public:
    explicit ChatUsersController(QObject *parent = nullptr);

    QVector<ChatUser> users() const;


signals:

    void userStateChangedConnected(const QString & userName);
    void userStateChangedDisconnected(const QString & userName);

    void connectedStateChanged(int userId, bool newState);

    void preUserAppended();
    void postUserAppended();

    void preUserRemoved(int userId);
    void postUserRemoved();

public slots:

    void updateOnlineUsers(const QStringList & onlineUsers);
    void updateConnectedUsers(const QStringList & connectedUsers);

private:

    QVector<ChatUser> mChatUsers;

    QStringList onlineUserNames() const;
    QStringList connectedUserNames() const;

    void appendUserOnDiscovery(const QString & newUser,
                               ChatUser::State state = ChatUser::State::DisconnectedUser);
    void removeUserOnOutdation(const QString & outdatedUser);

    void connectUserOnConnection(const QString & connectedUser);
    void disconnectUserOnDisconnection(const QString & disconnectedUser);
};

#endif // CHATUSERSCONTROLLER_H
