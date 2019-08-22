/*
#include "chatmessagesmodel.h"

#include <QDebug>

ChatMessagesModel::ChatMessagesModel(QObject *parent)
    : QAbstractListModel(parent), mpMessagesController(nullptr)
{
}

int ChatMessagesModel::rowCount(const QModelIndex &modelIndex) const
{
    if (modelIndex.isValid() || !mpMessagesController)
    {
        return 0;
    }

    return mpMessagesController->messages().size();
}

QVariant ChatMessagesModel::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid() || !mpMessagesController)
    {
        return QVariant();
    }

    const int messageId = modelIndex.row();

    const ChatMessage currentChatMessage =
            mpMessagesController->messages().at(messageId);

    switch(role)
    {
    case PublicationDateTimeRole:
        // Give the publication date & time
        return QVariant(currentChatMessage.mPublicationDateTime);
    case AuthorNameRole:
        // Give the name of the author
        return QVariant(currentChatMessage.mAuthorName);
    case ContentRole:
        // Give the content of the message
        return QVariant(currentChatMessage.mContent);
    case TypeRole:
        // Give the type of the message
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

    return QVariant();
}

QHash<int, QByteArray> ChatMessagesModel::roleNames() const

{
    QHash<int, QByteArray> roleNames;

    roleNames[PublicationDateTimeRole] = "publicationDateTime";
    roleNames[AuthorNameRole]          = "authorName";
    roleNames[ContentRole]             = "content";
    roleNames[TypeRole]                = "type";

    return roleNames;
}

ChatMessagesController * ChatMessagesModel::list() const

{
    return mpMessagesController;
}

void ChatMessagesModel::setList(ChatMessagesController * newList)
{
    beginResetModel();

    if (mpMessagesController)
    {
        mpMessagesController->disconnect(this);
    }

    mpMessagesController = newList;
    if (!mpMessagesController)
    {
        endResetModel();
        return;
    }

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

    endResetModel();
}
*/
