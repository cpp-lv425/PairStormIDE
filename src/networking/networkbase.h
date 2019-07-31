#ifndef SERVERDATA_H
#define SERVERDATA_H

#include <QString>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUdpSocket>
#include <QTcpServer>

#define CUSTOM_DEBUG

typedef quint16 PortNumType;
typedef quint64 SizeType;
const PortNumType g_defaultTcpPortNumber    = 36108;//36108
const PortNumType g_defaultUdpPortNumber    = 32807;
const SizeType    g_defaultBroadcastCycleMs = 500;

const QString     g_orgLabel             = "cpp-lv425";
const QString     g_appLabel             = "PairStorm 0.0.1";

struct ServerData {
    QString               m_name;
    QVector<QHostAddress> m_ips;
    QHostAddress          m_sourceIp;
    PortNumType           m_port;

    QString toJsonQString() const
    {
        // Create Json object and set its API
        QJsonObject json;
        json["app"] = g_appLabel;

        // Fill object with needed values
        json["name"] = m_name;
        QJsonArray jsonIpArr;
        for (const auto & ip : m_ips)
        {
            jsonIpArr.push_back(ip.toString());
        }
        json["ips"]   = jsonIpArr;
        json["port"] = m_port;

        // Compose the Json bearer string
        QJsonDocument doc(json);
        QByteArray docData = doc.toJson(QJsonDocument::Compact);
        return QString(docData);
    }

    void fromJsonQString(QString str)
    {
        // Decompose the Json bearer string
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject json = doc.object();

        // Verify if the Json object is not corrupted
        if(json.isEmpty()                 ||
           !json.keys().contains("app")   ||
           !json.keys().contains("ips")   ||
           !json.keys().contains("name")  ||
           !json.keys().contains("port"))
        {
            *this = ServerData();
            return;
        }

        // Check if the API version match
        if(json.value("app").toString() != g_appLabel) return;

        // Get needed values from Json object
        m_name = json.value("name").toString();
        m_port = static_cast<quint16>(json.value("port").toInt());
        QJsonArray jsonIpArr = json.value("ips").toArray();
        m_ips.clear();
        for (const auto ip : jsonIpArr)
        {
            QString str = ip.toString();
            m_ips.push_back(QHostAddress(str));
        }
    }

    bool empty() const
    {
        return m_name == QString();
    }
};

struct Segment {
    //QString      m_serverName;
    QString      m_data;
    QHostAddress m_ip;
    PortNumType  m_port;
};

struct Datagram {
    QString      m_data;
    QHostAddress m_ip;
    PortNumType  m_port;
};

#endif // SERVERDATA_H
