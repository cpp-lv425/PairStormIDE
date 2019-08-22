#include "chatmessagescontroller.h"
#include <QDebug>

ChatMessagesController::ChatMessagesController(const QString & authorName,
                                               QObject *parent) :
    QObject(parent),
    mAuthorName(authorName)
{
}

QVector<ChatMessage> ChatMessagesController::messages() const
{
    return mChatMessages;
}

void ChatMessagesController::sendGreetingsMessage()
{
    // Send greetings message from the Pair Storm appliction to the user
    ChatMessage greetingsMessage;

    greetingsMessage.mAuthorName = "PairStorm";
    greetingsMessage.mContent = "Welcome Pair Storm Application\nTo get help, type \"help\"";
    greetingsMessage.mPublicationDateTime = QDateTime::currentDateTimeUtc();
    greetingsMessage.mType = ChatMessage::Type::SystemMessage;

    appendMessage(greetingsMessage);
}

void ChatMessagesController::sendCanNotLogInTwiceMessage()
{
    // Send greetings message from the Pair Storm appliction to the user
    ChatMessage canNotLogInTwiceMessage;

    canNotLogInTwiceMessage.mAuthorName = "PairStorm";
    canNotLogInTwiceMessage.mContent = "Sorry, " + mAuthorName + ", but you can not log in since\nyou are already logged in";
    canNotLogInTwiceMessage.mPublicationDateTime = QDateTime::currentDateTimeUtc();
    canNotLogInTwiceMessage.mType = ChatMessage::Type::SystemMessage;

    appendMessage(canNotLogInTwiceMessage);
}

void ChatMessagesController::appendMessage(const ChatMessage & newMessage)
{
    if (newMessage.mType == ChatMessage::Type::UserMessage)
    {
        // Only users messages have to be added to the history
    }
    // TODO some stuff
    // if everything is fine, next part of code executes

    // Display message in the chat
    emit preMessageAppended();

    mChatMessages.append(newMessage);
    /*
    std::sort(mChatMessages.begin(),
              mChatMessages.end(),
              [](const ChatMessage & chatMessage1, const ChatMessage & chatMessge2)
              {
                  return chatMessage1.mPublicationDateTime < chatMessge2.mPublicationDateTime;
              });
    */
    emit postMessageAppended();
}

void ChatMessagesController::appendMessage(const QString & newMessageContent)
{
    ChatMessage newMessage;

    newMessage.mPublicationDateTime = QDateTime::currentDateTimeUtc();
    newMessage.mAuthorName          = this->mAuthorName;
    newMessage.mContent             = newMessageContent;
    newMessage.mType                = ChatMessage::Type::UserMessage;

    // Add created message in the backend and share it with others
    appendMessage(newMessage);
    emit sendingMessage(newMessage);
}
