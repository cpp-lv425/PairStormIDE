#include "chatmessagescontroller.h"
#include <QDebug>

ChatMessagesController::ChatMessagesController(const QString & authorName,
                                               QObject *parent) :
    QObject(parent),
    mAuthorName(authorName)
{
    // Send greetings message from the Pair Storm appliction to the user
    ChatMessage greetingsMessage;

    greetingsMessage.mAuthorName = "PairStorm";
    greetingsMessage.mContent = "Welcome Pair Storm Application\nTo get help, type \"help\"";
    greetingsMessage.mPublicationDateTime = QDateTime::currentDateTimeUtc();
    greetingsMessage.mType = ChatMessage::Type::SystemMessage;

    appendMessage(greetingsMessage);
}

QVector<ChatMessage> ChatMessagesController::messages() const
{
    return mChatMessages;
}

bool ChatMessagesController::appendMessage(const ChatMessage & newMessage)
{
    if (newMessage.mType == ChatMessage::Type::UserMessage)
    {
        // Only users messages have to be added to the history
    }
    // TODO some stuff
    // if everything is fine, next part of code executes

    emit preMessageAppended();
    mChatMessages.append(newMessage);
    emit postMessageAppended();

    return true;
}

void ChatMessagesController::appendMessage(const QString & newMessageContent)
{
    qDebug() << "ATTENTION";
    qDebug() << "someone appending message: " << newMessageContent;
    emit preMessageAppended();

    ChatMessage newMessage;

    newMessage.mPublicationDateTime = QDateTime::currentDateTimeUtc();
    newMessage.mAuthorName          = this->mAuthorName;
    newMessage.mContent             = newMessageContent;
    newMessage.mType                = ChatMessage::Type::UserMessage;

    mChatMessages.append(newMessage);

    emit postMessageAppended();
}
