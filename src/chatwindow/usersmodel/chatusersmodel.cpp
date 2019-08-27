#include "chatusersmodel.h"
// ==========================================================================================
// ==========================================================================================
//                                                                                CONSTRUCTOR
ChatUsersModel::ChatUsersModel(QObject *parent)
    : QAbstractListModel(parent), mpUsersController(nullptr)
{
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                    USERS LIST' SIZE GETTER
int ChatUsersModel::rowCount(const QModelIndex &modelIndex) const
{
    if (modelIndex.isValid() || !mpUsersController)
    {
        // Hold the damage if controller is not ready or model index is broken
        return 0;
    }

    return mpUsersController->users().size();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                           SEPARATE USERS ATTRIBUTES GETTER
QVariant ChatUsersModel::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid() || !mpUsersController)
    {
        // Hold the damage if controller is not ready or model index is broken
        return QVariant();
    }

    // Get reference to current user instance from users list
    const int userId = modelIndex.row();
    const ChatUser currentChatUser =
            mpUsersController->users().at(userId);

    // Switch upon attributes to be received
    switch(role)
    {
    case UserNameRole:
        // Return the name of the user
        return QVariant(currentChatUser.mUserName);
    case UserConnectedRole:
        // Return true if the user is connected user
        return QVariant(currentChatUser.mState ==
                        ChatUser::State::ConnectedUser);
    case UserOnlineRole:
        // Return true if user is online user
        return QVariant(currentChatUser.mState !=
                        ChatUser::State::OfflineUser);
    }
    // Hold the damage if attribute index is broken
    return QVariant();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                           SEPARATE USERS ATTRIBUTES SETTER
bool ChatUsersModel::setData(const QModelIndex &modelIndex,
                             const QVariant &newValue,
                             int role)
{
    if (!modelIndex.isValid() || !mpUsersController)
    {
        // Hold the damage if controller is not ready or model index is broken
        return false;
    }

    // Get reference to current user instance from users list
    const int userId = modelIndex.row();
    ChatUser currentUser =
            mpUsersController->users().at(userId);

    // Switch upon attributes to be changed
    switch(role)
    {
    case UserNameRole:
        // Idle
        break;
    case UserConnectedRole:
        // User can change only "isConnected" attribute
        if (newValue.toBool())
        {
            // User intends to connect neighbor
            if (currentUser.mState == ChatUser::State::ConnectedUser)
            {
                return true;
            }
            currentUser.mState = ChatUser::State::ConnectedUser;
            emit mpUsersController->userStateChangedConnected(currentUser.mUserName);
        }
        else
        {
            // User intends to disconnect neighbor
            if (currentUser.mState == ChatUser::State::DisconnectedUser)
            {
                return true;
            }
            currentUser.mState = ChatUser::State::DisconnectedUser;
            emit mpUsersController->userStateChangedDisconnected(currentUser.mUserName);
        }
        break;
    case UserOnlineRole:
        // Idle
        break;
    }

    if (data(modelIndex, role) != newValue) {
        // If data has been really changed
        emit dataChanged(modelIndex, modelIndex, QVector<int>({role}));
        return true;
    }
    return false;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                  ATTRIBUTES INDEXES GETTER
QHash<int, QByteArray> ChatUsersModel::roleNames() const
{
    // Fill the attributeID -> attributeNAME relation map
    QHash<int, QByteArray> roleNames;
    roleNames[UserConnectedRole] = "isUserConnected";
    roleNames[UserOnlineRole]    = "isUserOnline";
    roleNames[UserNameRole]      = "userName";

    return roleNames;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                              LOCAL USERS CONTROLLER GETTER
ChatUsersController* ChatUsersModel::list() const
{
    return mpUsersController;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                              LOCAL USERS CONTROLLER SETTER
void ChatUsersModel::setList(ChatUsersController *newController)
{
    beginResetModel();

    if (mpUsersController)
    {
        // Disconnect previous controller if it is not nullptr
        mpUsersController->disconnect(this);
    }

    mpUsersController = newController;
    if (mpUsersController)
    {
        // Connect brand new controller if it is not nullptr
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
    }

    endResetModel();
}

