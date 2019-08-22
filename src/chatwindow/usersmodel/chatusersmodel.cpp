#include "chatusersmodel.h"

#include <QDebug>

ChatUsersModel::ChatUsersModel(QObject * parent)
    : QAbstractListModel(parent), mpUsersController(nullptr)
{
}

int ChatUsersModel::rowCount(const QModelIndex & modelIndex) const
{
    if (modelIndex.isValid() || !mpUsersController)
    {
        return 0;
    }

    return mpUsersController->users().size();
}

QVariant ChatUsersModel::data(const QModelIndex & modelIndex, int role) const
{
    if (!modelIndex.isValid() || !mpUsersController)
    {
        return QVariant();
    }

    const int userId = modelIndex.row();

    const ChatUser currentChatUser =
            mpUsersController->users().at(userId);

    switch(role)
    {
    case UserNameRole:
        // Give the name of the user
        return QVariant(currentChatUser.mUserName);
    case UserConnectedRole:
        // Give info about user' connection
        return QVariant(currentChatUser.mState ==
                        ChatUser::State::ConnectedUser);
    case UserOnlineRole:
        // Give info about user' availability
        return QVariant(currentChatUser.mState !=
                        ChatUser::State::OfflineUser);
    }

    return QVariant();
}


bool ChatUsersModel::setData(const QModelIndex & modelIndex,
                             const QVariant & newValue,
                             int role)
{
    if (!modelIndex.isValid() || !mpUsersController)
    {
        return false;
    }

    const int userId = modelIndex.row();

    ChatUser currentUser =
            mpUsersController->users().at(userId);

    switch(role)
    {
    case UserNameRole:
        // Idle
        break;
    case UserConnectedRole:
        // Try to connect user
       if (newValue.toBool())
       {
           currentUser.mState = ChatUser::State::ConnectedUser;
           emit mpUsersController->userStateChangedConnected(currentUser.mUserName);
       }
       else
       {
           currentUser.mState = ChatUser::State::DisconnectedUser;
           emit mpUsersController->userStateChangedDisconnected(currentUser.mUserName);
       }

        break;
    case UserOnlineRole:
        // Idle
        break;
    }

    if (data(modelIndex, role) != newValue) {
        emit dataChanged(modelIndex, modelIndex, QVector<int>({role}));
        return true;
    }
    return false;
}

QHash<int, QByteArray> ChatUsersModel::roleNames() const

{
    QHash<int, QByteArray> roleNames;

    roleNames[UserConnectedRole] = "isUserConnected";
    roleNames[UserOnlineRole]    = "isUserOnline";
    roleNames[UserNameRole]      = "userName";

    return roleNames;
}

ChatUsersController * ChatUsersModel::list() const

{
    return mpUsersController;
}

void ChatUsersModel::setList(ChatUsersController * newList)
{
    beginResetModel();

    if (mpUsersController)
    {
        mpUsersController->disconnect(this);
    }

    mpUsersController = newList;
    if (!mpUsersController)
    {
        endResetModel();
        return;
    }

    connect(mpUsersController, &ChatUsersController::preUserAppended,
            this, [=]()
            {
                const int userRow = mpUsersController->users().size();
                beginInsertRows(QModelIndex(), userRow, userRow);
            }, Qt::AutoConnection);

    connect(mpUsersController, &ChatUsersController::postUserAppended,
            this, [=]()
            {
                endInsertRows();
            }, Qt::AutoConnection);

    connect(mpUsersController, &ChatUsersController::connectedStateChanged,
            this, [=](int index, bool isNowConnected)
            {
                if(this->setData(this->index(index), isNowConnected, UserConnectedRole))
                    qDebug() << "data successfully set";
            }, Qt::AutoConnection);

    connect(mpUsersController, &ChatUsersController::preUserRemoved,
            this, [=](int index)
            {
                beginRemoveRows(QModelIndex(), index, index);
            }, Qt::AutoConnection);

    connect(mpUsersController, &ChatUsersController::postUserRemoved,
            this, [=]()
            {
                endRemoveRows();
            }, Qt::AutoConnection);

    endResetModel();
}

