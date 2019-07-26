#ifndef SERVERDATA_H
#define SERVERDATA_H

#include <QString>
#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>


typedef quint16 PortNumType;
const PortNumType g_defaultTcpPortNumber = 36108;
const PortNumType g_defaultUdpPortNumber = 32807;

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


/*
#include <QObject>
#include <QUdpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <sstream>
#include <string>

struct ServerData
{
    std::string writeServerDataToStr() const {
        std::string str;
        QJsonObject json;
        json["name"] = m_serverName;
        json["ip"]   = m_serverIp;
        json["port"] = m_serverPort;
        QJsonDocument doc(json);
        QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);
        str = QLatin1String(docByteArray).tos;
        return str;
    }

    void getServerDataFromStr(std::string str) {
        ServerData serverData;
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject json;
        json = doc.object();
        m_serverName = json.value("name").toString();
        m_serverIp   = json.value("ip").toString();
        m_serverPort = json.value("port").toString();
    }

    std::string getServerName() const {
        return m_serverName;
    }

    QHostAddress getServerIp() const {

    }

    std::string getServerPort() const {
        auto str = std::stringstream(m_serverPort);

    }

private:
    std::string m_serverName;
    std::string m_serverIp;
    std::string m_serverPort;
};
*/
#endif // SERVERDATA_H
