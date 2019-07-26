#ifndef SERVERDATA_H
#define SERVERDATA_H

#include <QString>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUdpSocket>
#include <QTcpServer>


typedef quint16 PortNumType;
const PortNumType g_defaultTcpPortNumber = 36108;
const PortNumType g_defaultUdpPortNumber = 32807;

struct ServerData {
    QString m_name;
    QVector<QHostAddress> m_ips;
    PortNumType m_port;

    QString toJsonQString() const {
        QJsonArray jsonIpArr;
        for (const auto & ip : m_ips)
        {
            jsonIpArr.push_back(ip.toString());
        }
        QJsonObject json;
        json["name"] = m_name;
        json["ips"]   = jsonIpArr;
        json["port"] = m_port;
        QJsonDocument doc(json);

        QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);

        QString str;
        str = QLatin1String(docByteArray);
        return str;
    }

    void fromJsonQString(QString str) {
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject json = doc.object();
        QJsonArray jsonIpArr = json.value("ips").toArray();
        m_name = json.value("name").toString();
        m_port = static_cast<quint16>(json.value("port").toInt());
        m_ips.clear();
        for (const auto ip : jsonIpArr)
        {
            QString str = ip.toString();
            m_ips.push_back(QHostAddress(str));
        }
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
