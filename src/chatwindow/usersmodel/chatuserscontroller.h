#ifndef CHATUSERSCONTROLLER_H
#define CHATUSERSCONTROLLER_H

#include <QObject>
#include "userdb.h"
#include "chatuser.h"

// ==========================================================================================
//                                                                           USERS CONTROLLER
// ==========================================================================================
class ChatUsersController : public QObject
{
    Q_OBJECT

public:

    explicit ChatUsersController(const QString &userName, QObject *parent = nullptr);

    // Users list getter
    QVector<ChatUser> users() const;

signals:

    // Signals that user wants to start or stop sharing
    void userStateChangedConnected(const QString &);
    void userStateChangedDisconnected(const QString &);

    // Signals to users list model that inform about changes
    void preUserAppended();
    void postUserAppended();

    void preUserRemoved(int);
    void postUserRemoved();

public slots:

    // Updates list of online users
    void updateOnlineUsers(const QStringList &newOnlineUsers);
    // Updates online users "isConnected" attribute
    void updateConnectedUsers(const QStringList &newConnectedUsers);

private:

    // Bunch of current discovered users
    QVector<ChatUser> mChatUsers;

    UserDb            mDatabaseUsers;

    // Return lists of online users as well as connected users
    QStringList onlineUserNames() const;
    QStringList connectedUserNames() const;

    // Appends and removes user from users list
    void appendUser(const QString &newUserName,
                    ChatUser::State state = ChatUser::State::DisconnectedUser);
    void removeUser(const QString &outdatedUserName);

    // Connects or disconnects user
    void connectUser(const QString &connectedUser);
    void disconnectUser(const QString &disconnectedUser);
};

#endif // CHATUSERSCONTROLLER_H
