#include "chatmessagesmodel.h"
// ==========================================================================================
// ==========================================================================================
//                                                                                CONSTRUCTOR
ChatMessagesModel::ChatMessagesModel(QObject *parent)
    : QAbstractListModel(parent), mpMessagesController(nullptr)
{
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                 MESSAGES LIST' SIZE GETTER
int ChatMessagesModel::rowCount(const QModelIndex &modelIndex) const
{
    if (modelIndex.isValid() || !mpMessagesController)
    {
        // Hold the damage if controller is not ready or model index is broken
        return 0;
    }

    return mpMessagesController->messages().size();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                        SEPARATE MESSAGES ATTRIBUTES GETTER
QVariant ChatMessagesModel::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid() || !mpMessagesController)
    {
        // Hold the damage if controller is not ready or model index is broken
        return QVariant();
    }

    // Get reference to current message instance from messages list
    const int messageId = modelIndex.row();
    const ChatMessage currentChatMessage =
            mpMessagesController->messages().at(messageId);

    // Switch upon attributes to be received
    switch(role)
    {
    case PublicationDateTimeRole:
        // Return the publication date & time
        return QVariant(currentChatMessage.mPublicationDateTime);
    case AuthorNameRole:
        // Return the name of the author
        return QVariant(currentChatMessage.mAuthorName);
    case ContentRole:
        // Return the content of the message
        return QVariant(currentChatMessage.mContent);
    case TypeRole:
        // Return the type of the message
        QString messageType;
        switch (currentChatMessage.mType)
        {
        case ChatMessage::Type::UserMessage:
            messageType = "ordinary";
            break;
        case ChatMessage::Type::SystemMessage:
            messageType = "service";
            break;
        }
        return QVariant(messageType);
    }
    // Hold the damage if attribute index is broken
    return QVariant();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                  ATTRIBUTES INDEXES GETTER
QHash<int, QByteArray> ChatMessagesModel::roleNames() const
{
    // Fill the attributeID -> attributeNAME relation map
    QHash<int, QByteArray> roleNames;
    roleNames[PublicationDateTimeRole] = "publicationDateTime";
    roleNames[AuthorNameRole]          = "authorName";
    roleNames[ContentRole]             = "content";
    roleNames[TypeRole]                = "type";

    return roleNames;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                           LOCAL MESSAGES CONTROLLER GETTER
ChatMessagesController* ChatMessagesModel::list() const
{
    return mpMessagesController;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                           LOCAL MESSAGES CONTROLLER SETTER
void ChatMessagesModel::setList(ChatMessagesController *newController)
{
    beginResetModel();

    if (mpMessagesController)
    {
        // Disconnect previous controller if it is not nullptr
        mpMessagesController->disconnect(this);
    }

    mpMessagesController = newController;
    if (mpMessagesController)
    {
        // Connect brand new controller if it is not nullptr
        connect(mpMessagesController, &ChatMessagesController::preMessageAppended,
                this, [=]()
                {
                    const int elementRow = mpMessagesController->messages().size();
                    beginInsertRows(QModelIndex(), elementRow, elementRow);
                }, Qt::AutoConnection);

        connect(mpMessagesController, &ChatMessagesController::postMessageAppended,
                this, [=]()
                {
                    endInsertRows();
                }, Qt::AutoConnection);
    }

    endResetModel();
}
