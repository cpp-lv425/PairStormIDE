#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

//============================================================ General Qt libraries
#include <QString>
#include <QDateTime>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>


enum SystemMessage : int {
    CanNotLogInTwiceMessage,
    GreetingsMessage,
    DefaultMessage
};

const QString gcSystemName = "PairStorm";
const QHash<int, QString> gcSystemMessages {
    {
        SystemMessage::GreetingsMessage,
        QString("Welcome to PairStorm Application\nTo get help, type \"help\"")
    },
    {
        SystemMessage::CanNotLogInTwiceMessage,
        QString("Sorry, you can not log in since\nyou are already logged in")
    },
    {
        SystemMessage::DefaultMessage,
        QString("Hello from PairStorm")
    }
};

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
        if (json.isEmpty()                      ||
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
        QString appLabel =
                applicationSettings.value("applicationName").toString();
        if (json.value("app").toString() != appLabel)
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

#endif // CHATMESSAGE_H
