#include "connectionmanager.h"

#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "constantsdocumentationl.h"

ConnectionManager::ConnectionManager(QObject *parent)
    : QObject (parent)
{
}

bool ConnectionManager::hasConnection()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(ReferenceUrl::google);
    QNetworkReply* reply = manager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    return reply->bytesAvailable() ? true : false;
}
