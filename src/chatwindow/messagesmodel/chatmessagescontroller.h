#ifndef CHATMESSAGESCONTROLLER_H
#define CHATMESSAGESCONTROLLER_H

#include <QObject>
#include "chatmessage.h"

// ==========================================================================================
//                                                                        MESSAGES CONTROLLER
// ==========================================================================================
class ChatMessagesController : public QObject
{
    Q_OBJECT

public:

    explicit ChatMessagesController(const QString &authorName,
                                    QObject *parent = nullptr);

    // Messages collection getter
    QVector<ChatMessage> messages() const;

    // Service messages generators
    void sendSystemMessage(SystemMessage messageType);

signals:

    // Signal that user try to send message
    void sendingMessage(const ChatMessage &);

    // Signals to messages list model that inform about changes
    void preMessageAppended();
    void postMessageAppended();

public slots:

    // Appends incoming messages
    void appendMessage(const ChatMessage &newMessage);
    // Generates & appends incomming message based on author input
    void appendMessage(const QString &newUserMessageContent);

private:

    // Name of the User and bunch of current messages in the chat
    const QString        mcAuthorName;
    QVector<ChatMessage> mChatMessages;
};

#endif // CHATMESSAGESCONTROLLER_H
