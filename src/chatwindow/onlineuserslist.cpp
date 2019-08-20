#include "onlineuserslist.h"
#include <QDebug>

OnlineUsersList::OnlineUsersList(QObject *parent) : QObject(parent)
{
    ChatUser user;
    user.mUserName = "Bohdan Vasichkin";
    user.mState = ChatUser::State::DisconnectedUser;
    mChatUsers.append(user);

    user.mUserName = "ValentynFk";
    user.mState = ChatUser::State::ConnectedUser;
    mChatUsers.append(user);

    user.mUserName = "PeterLysyk";
    user.mState = ChatUser::State::DisconnectedUser;
    mChatUsers.append(user);

    user.mUserName = "PeterLysyk";
    user.mState = ChatUser::State::ConnectedUser;
    mChatUsers.append(user);

    user.mUserName = "PeterLysyk";
    user.mState = ChatUser::State::ConnectedUser;
    mChatUsers.append(user);

    user.mUserName = "PeterLysyk";
    user.mState = ChatUser::State::ConnectedUser;
    mChatUsers.append(user);
}

ChatUser & OnlineUsersList::at(const int & index)
{
    return mChatUsers[index];
}

int OnlineUsersList::size() const
{
    return mChatUsers.size();
}

void OnlineUsersList::updateUsers(const QStringList & userNames)
{
    if (mChatUsers.empty())
    {
        return;
    }
    // Remove users, whose names are not in userNames
    mChatUsers.erase(std::remove_if(mChatUsers.begin(),
                                    mChatUsers.end(),
                                    [userNames](ChatUser user)
                                    {
                                        return !userNames.contains(user.mUserName);
                                    }),
                     mChatUsers.end());
    // Add users, whose names are not present in mChatUsers
    QStringList knownUserNames;
    std::for_each(mChatUsers.cbegin(),
                  mChatUsers.cend(),
                  [&knownUserNames](const ChatUser & user)
                  {
                      knownUserNames.push_back(user.mUserName);
                  });
    std::for_each(userNames.cbegin(),
                  userNames.cend(),
                  [knownUserNames, this] (const QString & userName)
                  {
                      if (!knownUserNames.contains(userName))
                      {
                          ChatUser newUser;
                          newUser.mUserName = userName;
                          newUser.mState    = ChatUser::State::DisconnectedUser;
                          this->mChatUsers.append(newUser);
                      }
                  });
}

void OnlineUsersList::connectUsers(const QStringList & userNames)
{
    std::for_each(mChatUsers.begin(),
                  mChatUsers.end(),
                  [userNames](ChatUser & user)
                  {
                      user.mState = (userNames.contains(user.mUserName))?
                                  ChatUser::State::ConnectedUser   :
                                  ChatUser::State::DisconnectedUser;
                  });
}
