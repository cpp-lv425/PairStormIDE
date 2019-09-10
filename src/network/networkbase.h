#ifndef SERVERDATA_H
#define SERVERDATA_H

//=================================================================== STL libraries
#include <memory>
#include <algorithm>

//============================================================ General Qt libraries
#include <QObject>
#include <QSettings>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

//=================================================== Network interaction libraries
#include <QtNetwork>
#include <QHostInfo>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkSession>
#include <QNetworkInterface>


#define CUSTOM_DEBUG

// Custom types
typedef quint16 PortNumType;
typedef qint64  SizeType;

// Default values for common parameters
const PortNumType gDefaultTcpPortNumber    = 36108;
const PortNumType gDefaultUdpPortNumber    = 32807;

const SizeType    gDefaultConnTimeoutMs    = 1100;
const SizeType    gDefaultBroadcastCycleMs = 100;
const SizeType    gDefaultOutdatingCycleMs = 2000;


struct ServerData
{
    SizeType              mDiscoveryMoment;
    QHostAddress          mActiveIp;
    QString               mName;
    PortNumType           mPort;
    QVector<QHostAddress> mIps;

    QString toJsonQString() const
    {
        // Create Json object and set its API
        QJsonObject jsonAttrib;
        QSettings applicationSettings;
        QString appLabel = applicationSettings.value("applicationName").toString();
        jsonAttrib["app"] = appLabel;

        // Fill object with needed values
        jsonAttrib["name"] = mName;
        QJsonArray jsonIpArr;
        for (const auto &ip : mIps)
        {
            jsonIpArr.push_back(ip.toString());
        }
        jsonAttrib["ips"]  = jsonIpArr;
        jsonAttrib["port"] = mPort;

        // Compose the Json bearer string
        QJsonDocument attribDocument(jsonAttrib);
        QByteArray attribData(attribDocument.toJson(QJsonDocument::Compact));
        return QString(attribData);
    }

    void fromJsonQString(const QString &attribData)
    {
        *this = ServerData();
        // Decompose the Json bearer string
        QJsonDocument doc =
                QJsonDocument::fromJson(attribData.toUtf8());
        QJsonObject jsonAttrib = doc.object();

        // Verify if the Json object is not corrupted
        if (jsonAttrib.isEmpty()                 ||
            !jsonAttrib.keys().contains("app")   ||
            !jsonAttrib.keys().contains("ips")   ||
            !jsonAttrib.keys().contains("name")  ||
            !jsonAttrib.keys().contains("port"))
        {
            return;
        }
        // Check if the API version match
        QSettings applicationSettings;
        QString appLabel = applicationSettings.value("applicationName").toString();
        if (jsonAttrib.value("app").toString() != appLabel)
        {
            return;
        }

        // Get needed values from Json object
        mName = jsonAttrib.value("name").toString();
        mPort = static_cast<quint16>(jsonAttrib.value("port").toInt());
        QJsonArray jsonIpArr = jsonAttrib.value("ips").toArray();
        for (const auto ip : jsonIpArr)
        {
            mIps.push_back(QHostAddress(ip.toString()));
        }
    }

    bool empty() const
    {
        return mName == QString();
    }
};

struct NetworkMessage
{
    QString mSourceName;
    QString mContent;
    enum Type : int
    {
        StartSharingMessage,
        StopSharingMessage,
        ChangesMessage,
        ChatMessage
    } mType;

    QString toJsonQString() const
    {
        // Create Json object and set its API
        QJsonObject jsonAttrib;
        QSettings applicationSettings;
        QString appLabel = applicationSettings.value("applicationName").toString();
        jsonAttrib["app"] = appLabel;

        // Fill object with needed values
        jsonAttrib["sourceName"] = mSourceName;
        jsonAttrib["data"]       = mContent;
        jsonAttrib["type"]       = static_cast<int>(mType);

        // Compose the Json bearer string
        QJsonDocument attribDocument(jsonAttrib);
        QByteArray attribData = attribDocument.toJson(QJsonDocument::Compact);
        return QString(attribData);
    }

    void fromJsonQString(QString str)
    {
        *this = NetworkMessage();
        // Decompose the Json bearer string
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject json = doc.object();

        // Verify if the Json object is not corrupted
        if (json.isEmpty()                      ||
            !json.keys().contains("app")        ||
            !json.keys().contains("sourceName") ||
            !json.keys().contains("data")       ||
            !json.keys().contains("type"))
        {
            return;
        }
        // Check if the API version match
        QSettings applicationSettings;
        QString appLabel = applicationSettings.value("applicationName").toString();
        if (json.value("app").toString() != appLabel)
        {
            return;
        }

        // Get needed values from Json object
        mSourceName = json.value("sourceName").toString();
        mContent    = json.value("data").toString();
        mType       = Type(json.value("type").toInt());
    }

    bool empty() const
    {
        return mSourceName == QString();
    }
};

struct Segment {
    QString      mContent;
    PortNumType  mPort;
    QHostAddress mIp;
};

struct Datagram {
    QString      mContent;
    PortNumType  mPort;
    QHostAddress mIp;
};

#endif // SERVERDATA_H
