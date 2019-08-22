#ifndef CHATMESSAGESCONTROLLER_H
#define CHATMESSAGESCONTROLLER_H

#include <QObject>
#include <QDateTime>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>

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

    QString toJsonQString() const
    {
        // Create Json object and set its API
        QJsonObject jsonAttrib;
        QSettings applicationSettings;
        QString appLabel = applicationSettings.value("applicationName").toString();
        jsonAttrib["app"] = appLabel;

        // Fill object with needed values
        jsonAttrib["authorName"] = mAuthorName;
        jsonAttrib["data"]       = mContent;
        jsonAttrib["time"]       = mPublicationDateTime.toString("yyyy-MM-dd HH:mm:ss");
        jsonAttrib["type"]       = static_cast<int>(mType);

        // Compose the Json bearer string
        QJsonDocument attribDocument(jsonAttrib);
        QByteArray attribData = attribDocument.toJson(QJsonDocument::Compact);
        return QString(attribData);
    }

    void fromJsonQString(QString str)
    {
        *this = ChatMessage();
        // Decompose the Json bearer string
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject json = doc.object();

        // Verify if the Json object is not corrupted
        if(json.isEmpty()                      ||
           !json.keys().contains("app")        ||
           !json.keys().contains("authorName") ||
           !json.keys().contains("data")       ||
           !json.keys().contains("time")       ||
           !json.keys().contains("type"))
        {
            return;
        }
        // Check if the API version match
        QSettings applicationSettings;
        QString appLabel = applicationSettings.value("applicationName").toString();
        if(json.value("app").toString() != appLabel)
        {
            return;
        }

        // Get needed values from Json object
        mAuthorName = json.value("authorName").toString();
        mContent    = json.value("data").toString();
        mPublicationDateTime = QDateTime::fromString(json.value("time").toString(),
                                                     "yyyy-MM-dd HH:mm:ss");
        mType       = Type(json.value("type").toInt());
    }

    bool empty() const
    {
        return mAuthorName == QString();
    }
};

class ChatMessagesController : public QObject
{
    Q_OBJECT

public:
    explicit ChatMessagesController(const QString & authorName,
                                    QObject *parent = nullptr);

    QVector<ChatMessage> messages() const;

    void sendGreetingsMessage();
    void sendCanNotLogInTwiceMessage();

signals:

    void sendingMessage(const ChatMessage & newMessage);
    void preMessageAppended();
    void postMessageAppended();

public slots:

    void appendMessage(const ChatMessage & newMessage);

    void appendMessage(const QString & newMessageContent);

private:

    QVector<ChatMessage> mChatMessages;
    const QString mAuthorName;
};

#endif // CHATMESSAGESCONTROLLER_H
