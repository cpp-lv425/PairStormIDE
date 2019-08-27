#include "chatmessagescontroller.h"
// ==========================================================================================
// ==========================================================================================
//                                                        CONSTRUCTOR: INTIALIZES AUTHOR NAME
ChatMessagesController::ChatMessagesController(const QString &authorName,
                                               QObject *parent) :
    QObject(parent), mcAuthorName(authorName)
{
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                       MESSAGES LIST GETTER
QVector<ChatMessage> ChatMessagesController::messages() const
{
    return mChatMessages;
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                  SYSTEM' MESSAGE GENERATOR
void ChatMessagesController::sendSystemMessage(SystemMessage messageType)
{
    // Send certain message from Pair Storm appliction to the user
    ChatMessage systemMessage;
    systemMessage.mAuthorName = gcSystemName;
    systemMessage.mContent = gcSystemMessages[messageType];
    systemMessage.mPublicationDateTime = QDateTime::currentDateTimeUtc();
    systemMessage.mType = ChatMessage::Type::SystemMessage;

    appendMessage(systemMessage);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                              APPEND INGOING MESSAGES TO THE HISTORY & CHAT
void ChatMessagesController::appendMessage(const ChatMessage &newMessage)
{
    if (newMessage.mType == ChatMessage::Type::UserMessage)
    {
        // Only users messages have to be added to the history
        // TODO: add messages to the history in DB
    }

    // Display message in the chat
    emit preMessageAppended();
    mChatMessages.append(newMessage);
    emit postMessageAppended();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                          BUILD MESSAGE UPON USER' INPUT, APPEND IT & SHARE
void ChatMessagesController::appendMessage(const QString &newUserMessageContent)
{
    ChatMessage newUserMessage;
    newUserMessage.mPublicationDateTime = QDateTime::currentDateTimeUtc();
    newUserMessage.mAuthorName          = this->mcAuthorName;
    newUserMessage.mContent             = newUserMessageContent;
    newUserMessage.mType                = ChatMessage::Type::UserMessage;

    // Append message, created by the user, and share it with others
    appendMessage(newUserMessage);
    emit sendingMessage(newUserMessage);
}
