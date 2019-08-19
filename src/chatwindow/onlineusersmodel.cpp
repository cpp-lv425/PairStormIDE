#include "onlineusersmodel.h"
#include "onlineuserslist.h"
#include <QDebug>

OnlineUsersModel::OnlineUsersModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mpOnlineUsersList = new OnlineUsersList();
}

int OnlineUsersModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mpOnlineUsersList)
        return 0;

    qDebug() << "someone gets number of elements";
    return mpOnlineUsersList->users().size();
}

QVariant OnlineUsersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mpOnlineUsersList)
        return QVariant();

    qDebug() << "someone gets data";

    const OnlineChatUser user = mpOnlineUsersList->users().at(index.row());
    switch(role) {
    case ConnectedRole:
        return QVariant(user.mConnected);
    case NameRole:
        return QVariant(user.mUserName);
    }
    return QVariant();
}

bool OnlineUsersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mpOnlineUsersList)
    {
        return false;
    }


    OnlineChatUser user = mpOnlineUsersList->users().at(index.row());
    qDebug() << "somone try to set data on user with index " << index.row();
    switch(role) {
    case ConnectedRole:
        user.mConnected = value.toBool();
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
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> OnlineUsersModel::roleNames() const
{
    QHash<int, QByteArray> userNames;
    userNames[ConnectedRole] = "connected";
    userNames[NameRole] = "name";
    return userNames;
}

OnlineUsersList *OnlineUsersModel::getMpOnlineUsersList() const
{
    return mpOnlineUsersList;
}

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
