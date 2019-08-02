#include "connectionmanager.h"

#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

ConnectionManager::ConnectionManager(QObject *parent)
    : QObject (parent)
{
}

bool ConnectionManager::hasConnection()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("http://www.google.com"));
    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->bytesAvailable())
    {
        return true;
    }
    else
    {
        return false;
    }
}
