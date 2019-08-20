#include "onlineusersmodel.h"
#include "onlineuserslist.h"
#include "chatbase.h"
#include <QDebug>

OnlineUsersModel::OnlineUsersModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mpUsersList = new OnlineUsersList();

}

int OnlineUsersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !mpUsersList)
    {
        return 0;
    }

    int rowsNum = mpUsersList->size();
    return rowsNum;
}

QVariant OnlineUsersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mpUsersList)
    {
        return QVariant();
    }

    int userNum = index.row();
    const ChatUser currentUser
            = mpUsersList->at(userNum);

    switch(role)
    {
    case Role::NameRole:
        return QVariant(currentUser.mUserName);
    case Role::ConnectedRole:
        return QVariant(currentUser.mState);
    }

    return QVariant();
}

bool OnlineUsersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mpUsersList)
    {
        return false;
    }

    ChatUser& user = mpUsersList->at(index.row());
    qDebug() << "somone try to set data on user with index " << index.row();
    switch(role) {
    case ConnectedRole:
        if (value.toBool())
        {
            qDebug() << "connect to user: " << user.mUserName;
            user.mState = ChatUser::State::ConnectedUser;
            emit stateChangedOff(user.mUserName);
        }
        else
        {
            qDebug() << "disconnect from user: " << user.mUserName;
            user.mState = ChatUser::State::DisconnectedUser;
            emit stateChangedOff(user.mUserName);
        }
        break;
    case NameRole:
        user.mUserName = value.toString();
        break;
    }

    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags OnlineUsersModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEditable;
}

void OnlineUsersModel::updateOnlineUsers(const QStringList &userNames)
{
    mpUsersList->updateUsers(userNames);
}

void OnlineUsersModel::updateConnectedUsers(const QStringList &userNames)
{
    mpUsersList->connectUsers(userNames);
}

QHash<int, QByteArray> OnlineUsersModel::roleNames() const
{
    QHash<int, QByteArray> userNames;
    userNames[Role::NameRole]      = "name";
    userNames[Role::ConnectedRole] = "connected";
    return userNames;
}
/*
void OnlineUsersModel::setMpOnlineUsersList(OnlineUsersList *value)
{
    beginResetModel();

    if (mpOnlineUsersList)
        mpOnlineUsersList->disconnect(this);

    mpOnlineUsersList = value;

    if(mpOnlineUsersList)
    {
        connect(mpOnlineUsersList, &OnlineUsersList::preUserAppeared,
                this, [=]() {
            const int index = mpOnlineUsersList->users().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mpOnlineUsersList, &OnlineUsersList::postUserAppeared,
                this, [=]() {
            endInsertRows();
        });
        connect(mpOnlineUsersList, &OnlineUsersList::preUserDisappeared,
                this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mpOnlineUsersList, &OnlineUsersList::postUserDisappeared,
                this, [=]() {
            endRemoveRows();
        });

    }
    endResetModel();
}
*/
