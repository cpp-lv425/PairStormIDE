#include "chatuserscontroller.h"
// ==========================================================================================
// ==========================================================================================
//                                                                                CONSTRUCTOR
ChatUsersController::ChatUsersController(const QString &userName, QObject *parent) :
    QObject(parent), mDatabaseUsers()
{
    QVector<User> superUsers = mDatabaseUsers.getAllUsersFromDb();
    User currentUser (userName);
    mDatabaseUsers.addUserToDb(currentUser);
    // TODO fill chat users & implement additional states behavior
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                          USERS LIST GETTER
QVector<ChatUser> ChatUsersController::users() const
{
    return mChatUsers;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                               ONLINE USERS NAMES GENERATOR
QStringList ChatUsersController::onlineUserNames() const
{
    QStringList userNames;
    std::for_each(mChatUsers.cbegin(),
                  mChatUsers.cend(),
                  [&userNames](const ChatUser &chatUser)
                  {
                      userNames.push_back(chatUser.mUserName);
                  });
    return userNames;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                            CONNECTED USERS NAMES GENERATOR
QStringList ChatUsersController::connectedUserNames() const
{
    QStringList userNames;
    std::for_each(mChatUsers.cbegin(),
                  mChatUsers.cend(),
                  [&userNames](const ChatUser &chatUser)
                  {
                      if (chatUser.mState == ChatUser::State::ConnectedUser)
                      {
                          userNames.push_back(chatUser.mUserName);
                      }
                  });
    return userNames;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                       UPDATES ONLINE USERS
void ChatUsersController::updateOnlineUsers(const QStringList &newOnlineUsers)
{
    QStringList oldOnlineUsers = onlineUserNames();
    // Remove disappeared users
    std::for_each(oldOnlineUsers.cbegin(),
                  oldOnlineUsers.cend(),
                  [newOnlineUsers, this](const QString &userName)
                  {
                      if (!newOnlineUsers.contains(userName))
                      {
                          this->removeUser(userName);
                      }
                  });
    // Append new discovered users
    std::for_each(newOnlineUsers.cbegin(),
                  newOnlineUsers.cend(),
                  [oldOnlineUsers, this](const QString &userName)
                  {
                      if (!oldOnlineUsers.contains(userName))
                      {
                          this->appendUser(userName);
                      }
                  });
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                    UPDATES CONNECTED USERS
void ChatUsersController::updateConnectedUsers(const QStringList &newConnectedUsers)
{
    QStringList oldConnectedUsers = connectedUserNames();
    // Remove broken connections
    std::for_each(oldConnectedUsers.cbegin(),
                  oldConnectedUsers.cend(),
                  [newConnectedUsers, this](const QString &userName)
                  {
                      if (!newConnectedUsers.contains(userName))
                      {
                          this->disconnectUser(userName);
                      }
                  });
    // Append new connections
    std::for_each(newConnectedUsers.cbegin(),
                  newConnectedUsers.cend(),
                  [oldConnectedUsers, this](const QString &userName)
                  {
                      if (!oldConnectedUsers.contains(userName))
                      {
                          this->connectUser(userName);
                      }
                  });
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                     APPENDS USER WITH SPECIFIED ATTRIBUTES
void ChatUsersController::appendUser(const QString &newUserName, ChatUser::State state)
{
    // Append User with given userName and userState
    ChatUser newUser;
    newUser.mUserName = newUserName;
    newUser.mState    = state;

    emit preUserAppended();
    mChatUsers.append(newUser);
    emit postUserAppended();

    // Append User with given userName to database
    User newDBUser(newUserName);
    mDatabaseUsers.addUserToDb(newDBUser);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                     REMOVES SPECIFIED USER
void ChatUsersController::removeUser(const QString &outdatedUserName)
{
    auto userPos =
            std::find_if(mChatUsers.begin(),
                         mChatUsers.end(),
                         [outdatedUserName](const ChatUser &chatUser)
                         {
                             return outdatedUserName == chatUser.mUserName;
                         });

    if (userPos != mChatUsers.end())
    {
        // Remove user if found
        int userId = static_cast<int>(std::distance(mChatUsers.begin(), userPos));

        emit preUserRemoved(userId);
        mChatUsers.erase(userPos);
        emit postUserRemoved();
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                 CHANGES SPECIFIED USER' STATE TO CONNECTED
void ChatUsersController::connectUser(const QString &connectedUser)
{
    removeUser(connectedUser);
    appendUser(connectedUser, ChatUser::State::ConnectedUser);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                              CHANGES SPECIFIED USER' STATE TO DISCONNECTED
void ChatUsersController::disconnectUser(const QString &disconnectedUser)
{
    removeUser(disconnectedUser);
    appendUser(disconnectedUser, ChatUser::State::DisconnectedUser);
}
