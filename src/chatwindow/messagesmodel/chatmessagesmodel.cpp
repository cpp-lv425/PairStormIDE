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
        if (!mpMessagesController)
            qDebug() << "messages controller break row count";
        else
            qDebug() << "model indeex is not valid whilecouting rows";
        return 0;
    }

    qDebug() << "someone try to get size of the list";
    qDebug() << "size is: " << mpMessagesController->messages().size();

    return mpMessagesController->messages().size();
}

QVariant ChatMessagesModel::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid() || !mpMessagesController)
    {
        if (!mpMessagesController)
            qDebug() << "messages controller break data capturing";
        else
            qDebug() << "model indeex is not valid while data capturing";
        return QVariant();
    }

    qDebug() << "try to get row index";
    const int messageId = modelIndex.row();
    qDebug() << "row index is: " << messageId;

    const ChatMessage currentChatMessage =
            mpMessagesController->messages().at(messageId);

    qDebug() << "try to get message: " << currentChatMessage.mContent;
    qDebug() << "from: " << currentChatMessage.mAuthorName;
    qDebug() << "date: " << currentChatMessage.mPublicationDateTime;

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
    qDebug() << "try to set list";
    qDebug() << "size: " << newList->messages().size();
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
