#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include <QObject>
#include <QDateTime>

struct ChatMessage
{
    QString   mAuthorName;
    QString   mContent;
    QDateTime mPublicationDateTime;
    enum Type : int
    {
        SystemMessage,
        UserMessage
    } mType;
};

class ChatMessagesController : public QObject
{
    Q_OBJECT

public:
    explicit ChatMessagesController(const QString & authorName,
                                    QObject *parent = nullptr);

    QVector<ChatMessage> messages() const;

signals:

    void preMessageAppended();
    void postMessageAppended();

public slots:

    bool appendMessage(const ChatMessage & newMessage);

    void appendMessage(const QString & newMessageContent);

private:

    QVector<ChatMessage> mChatMessages;
    const QString mAuthorName;
};

#endif // MESSAGESMODEL_H
