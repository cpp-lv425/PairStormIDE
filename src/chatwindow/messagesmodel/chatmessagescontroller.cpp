#include "chatmessagescontroller.h"
// ==========================================================================================
// ==========================================================================================
//                                                        CONSTRUCTOR: INTIALIZES AUTHOR NAME
ChatMessagesController::ChatMessagesController(const QString &authorName,
                                               QObject *parent) :
    QObject(parent), mcAuthorName(authorName), mDatabaseMessages()
{
    QDateTime fromDateTime = QDateTime::currentDateTimeUtc().addDays(-2);
    QVector<Message> databaseMessages =
            mDatabaseMessages.getMessageFromDb(fromDateTime.toString("yyyy-MM-dd HH:mm:ss"));

    std::for_each(databaseMessages.cbegin(),
                  databaseMessages.cend(),
                  [this](const Message & previouslySavedMessage)
                  {
                      if (previouslySavedMessage.mBody != QString() &&
                          previouslySavedMessage.mUser != QString())
                      {
                          ChatMessage newChatMessage;
                          newChatMessage.mAuthorName          = previouslySavedMessage.mUser;
                          newChatMessage.mContent             = previouslySavedMessage.mBody;
                          newChatMessage.mPublicationDateTime =
                                  QDateTime::fromString(previouslySavedMessage.mTime,
                                                        "yyyy-MM-dd HH:mm:ss");
                          newChatMessage.mType                = ChatMessage::Type::UserMessage;

                          this->mChatMessages.append(newChatMessage);
                      }
                  });
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
    systemMessage.mAuthorName          = gcSystemName;
    systemMessage.mContent             = gcSystemMessages[messageType];
    systemMessage.mPublicationDateTime = QDateTime::currentDateTimeUtc();
    systemMessage.mType                = ChatMessage::Type::SystemMessage;

    appendMessage(systemMessage);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                              APPEND INGOING MESSAGES TO THE HISTORY & CHAT
void ChatMessagesController::appendMessage(const ChatMessage &newMessage)
{
    // Display message in the chat
    emit preMessageAppended();
    mChatMessages.append(newMessage);
    emit postMessageAppended();

    if (newMessage.mType == ChatMessage::Type::UserMessage)
    {
        // Only users messages have to be added to the history
        QString publicationTime =
                newMessage.mPublicationDateTime.toString("yyyy-MM-dd HH:mm:ss");
        Message newDBMessage(newMessage.mContent, newMessage.mAuthorName, publicationTime);

        mDatabaseMessages.addMessageToDb(newDBMessage);
    }
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
