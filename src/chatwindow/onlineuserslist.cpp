#include "onlineuserslist.h"
#include <QDebug>

OnlineUsersList::OnlineUsersList(QObject *parent) : QObject(parent)
{
    OnlineChatUser user;
    user.mConnected = false;
    user.mUserName = "Bohdan Vasichkin";
    mOnlineChatUsers.append(user);

    user.mConnected = true;
    user.mUserName = "ValentynFk";
    mOnlineChatUsers.append(user);

    user.mConnected = true;
    user.mUserName = "ValentynFk";
    mOnlineChatUsers.append(user);

    user.mConnected = true;
    user.mUserName = "ValentynFk";
    mOnlineChatUsers.append(user);

    user.mConnected = true;
    user.mUserName = "ValentynFk";
    mOnlineChatUsers.append(user);

    user.mConnected = true;
    user.mUserName = "ValentynFk";
    mOnlineChatUsers.append(user);

    user.mConnected = true;
    user.mUserName = "ValentynFk";
    mOnlineChatUsers.append(user);

    user.mConnected = true;
    user.mUserName = "ValentynFk";
    mOnlineChatUsers.append(user);
}

QVector<OnlineChatUser> OnlineUsersList::users() const
{
    return mOnlineChatUsers;
}

void OnlineUsersList::updateOnlineUsersOnChanges(const QStringList &userNames)
{
    // Remove users that are not online if any
    for (const auto & user : mOnlineChatUsers) {
        if (!userNames.contains(user.mUserName))
        {
            auto pos = std::find_if(mOnlineChatUsers.begin(),
                                    mOnlineChatUsers.end(),
                                    [=] (OnlineChatUser onlineUser) {
                return onlineUser.mUserName == user.mUserName;
            });
            if (pos != mOnlineChatUsers.end())
            {
                preUserDisappeared(static_cast<int>(std::distance(mOnlineChatUsers.begin(), pos)));
                mOnlineChatUsers.erase(pos);
                postUserDisappeared();
            }
        }
    }

    QStringList oldUserNames;
    for (const auto & user : mOnlineChatUsers) {
        oldUserNames.append(user.mUserName);
    }

    for (const auto & newUserName : userNames)
    {
        if(!oldUserNames.contains(newUserName))
        {
            preUserAppeared();
            OnlineChatUser newUser;
            newUser.mUserName = newUserName;
            newUser.mConnected = false;
            mOnlineChatUsers.append(newUser);
            postUserAppeared();
        }
    }
}

void OnlineUsersList::updateConnectedUsersOnChanges(const QStringList &userNames)
{
    // TODO
}

void OnlineUsersList::connectToUserOnClick()
{
    qDebug() << "try to connect to user ";
}
