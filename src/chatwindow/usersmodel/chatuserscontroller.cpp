#include "chatuserscontroller.h"
#include <QDebug>

ChatUsersController::ChatUsersController(QObject *parent) :
    QObject(parent)
{
}

QVector<ChatUser> ChatUsersController::users() const
{
    return mChatUsers;
}

QStringList ChatUsersController::onlineUserNames() const
{
    QStringList userNames;
    std::for_each(mChatUsers.cbegin(),
                  mChatUsers.cend(),
                  [&userNames](const ChatUser & chatUser)
                  {
                      userNames.push_back(chatUser.mUserName);
                  });
    return userNames;
}

QStringList ChatUsersController::connectedUserNames() const
{
    QStringList userNames;
    std::for_each(mChatUsers.cbegin(),
                  mChatUsers.cend(),
                  [&userNames](const ChatUser & chatUser)
                  {
                      if (chatUser.mState == ChatUser::State::ConnectedUser)
                      {
                          userNames.push_back(chatUser.mUserName);
                      }
                  });
    return userNames;
}

void ChatUsersController::updateOnlineUsers(const QStringList &onlineUsers)
{
    QStringList oldOnlineUsers = onlineUserNames();
    // Remove disappeared users
    std::for_each(oldOnlineUsers.cbegin(),
                  oldOnlineUsers.cend(),
                  [onlineUsers, this](const QString & userName)
                  {
                      if (!onlineUsers.contains(userName))
                      {
                          this->removeUserOnOutdation(userName);
                      }
                  });
    // Append new discovered users
    std::for_each(onlineUsers.cbegin(),
                  onlineUsers.cend(),
                  [oldOnlineUsers, this](const QString & userName)
                  {
                      if (!oldOnlineUsers.contains(userName))
                      {
                          this->appendUserOnDiscovery(userName);
                      }
                  });
}

void ChatUsersController::updateConnectedUsers(const QStringList &connectedUsers)
{
    QStringList oldConnectedUsers = connectedUserNames();
    // Remove broken connections
    std::for_each(oldConnectedUsers.cbegin(),
                  oldConnectedUsers.cend(),
                  [connectedUsers, this](const QString & userName)
                  {
                      if (!connectedUsers.contains(userName))
                      {
                          this->disconnectUserOnDisconnection(userName);
                      }
                  });
    // Append new connections
    std::for_each(connectedUsers.cbegin(),
                  connectedUsers.cend(),
                  [oldConnectedUsers, this](const QString & userName)
                  {
                      if (!oldConnectedUsers.contains(userName))
                      {
                          this->connectUserOnConnection(userName);
                      }
                  });
}

void ChatUsersController::appendUserOnDiscovery(const QString & newUserName, ChatUser::State state)
{
    emit preUserAppended();

    ChatUser newUser;
    newUser.mUserName = newUserName;
    newUser.mState = state;
    mChatUsers.append(newUser);
/*
    std::stable_sort(mChatUsers.begin(),
                     mChatUsers.end(),
                     [](const ChatUser & chatUser1, const ChatUser & chatUser2)
                     {
                         bool isConnected1 =
                                 (chatUser1.mState == ChatUser::State::ConnectedUser);
                         bool isDisconnected2 =
                                 (chatUser2.mState == ChatUser::State::DisconnectedUser);
                         return isConnected1 || isDisconnected2;
                     });
*/
    emit postUserAppended();
}

void ChatUsersController::removeUserOnOutdation(const QString & outdatedUserName)
{
    auto userPos =
            std::find_if(mChatUsers.begin(),
                         mChatUsers.end(),
                         [outdatedUserName](const ChatUser & chatUser)
                         {
                             return outdatedUserName == chatUser.mUserName;
                         });

    if (userPos == mChatUsers.end())
    {
        return;
    }

    int userId = static_cast<int>(std::distance(mChatUsers.begin(), userPos));

    emit preUserRemoved(userId);

    mChatUsers.erase(userPos);

    emit postUserRemoved();
}

void ChatUsersController::connectUserOnConnection(const QString &connectedUser)
{
    removeUserOnOutdation(connectedUser);
    appendUserOnDiscovery(connectedUser, ChatUser::State::ConnectedUser);
    /*
    auto userPos =
            std::find_if(mChatUsers.begin(),
                         mChatUsers.end(),
                         [connectedUser](const ChatUser & chatUser)
                         {
                             return connectedUser == chatUser.mUserName;
                         });

    if (userPos == mChatUsers.end())
    {
        return;
    }

    int userId = static_cast<int>(std::distance(mChatUsers.begin(), userPos));
    bool isConnected = true;
    emit connectedStateChanged(userId, isConnected);
    */
}

void ChatUsersController::disconnectUserOnDisconnection(const QString &disconnectedUser)
{
    removeUserOnOutdation(disconnectedUser);
    appendUserOnDiscovery(disconnectedUser, ChatUser::State::DisconnectedUser);
    /*
    auto userPos =
            std::find_if(mChatUsers.begin(),
                         mChatUsers.end(),
                         [disconnectedUser](const ChatUser & chatUser)
                         {
                             return disconnectedUser == chatUser.mUserName;
                         });

    if (userPos == mChatUsers.end())
    {
        return;
    }

    int userId = static_cast<int>(std::distance(mChatUsers.begin(), userPos));
    bool isConnected = false;
    emit connectedStateChanged(userId, isConnected);
    */
}
