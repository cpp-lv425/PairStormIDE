#include "downloader.h"

#include <QtNetwork>

Downloader::Downloader(QObject* pobj) : QObject(pobj)
{   //qDebug() << "Downloader";
    mNam = new QNetworkAccessManager(this);
    connect(mNam, &QNetworkAccessManager::finished, this,   &Downloader::onFinished);
    //qDebug() << "/Downloader";
}

void Downloader::download(const QUrl& url)
{   //qDebug() << "download" << url;
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "curl/7.58.0");
    //request.setRawHeader("User-Agent", "MarsLviv");
    request.setRawHeader("Authorization", "token 332b5f9145df8ae30d511ae4c6a92f48491b068b");

    QString username = "MarsLviv";
    QString password = "220iopMS";
    QString credentials = username + ":" + password;
    QByteArray data = credentials.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    //request.setRawHeader("Authorization", headerData.toLocal8Bit());

    QNetworkReply*  pnr = mNam->get(request);
    Q_UNUSED(pnr)
}

void Downloader::downloadGET(const QUrl &url, const QString &userName,
                             const QString &tokenOrPassword)
{   //qDebug() << "Downloader::downloadGET";
    QNetworkRequest request(url);

    request.setRawHeader("User-Agent", "curl/7.58.0");
    //request.setRawHeader("User-Agent", userName.toLocal8Bit());

    if (tokenOrPassword.size() == 0)        // not authorized request
    {

    }
    else if (tokenOrPassword.size() == 40)   // request via token
    {
        QString tokenStr("token " + tokenOrPassword);
        request.setRawHeader("Authorization", tokenStr.toLocal8Bit());
    }
    else                                    // request via userName:password
    {
        QString credentials = userName + ":" + tokenOrPassword;
        QByteArray data = credentials.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;
        request.setRawHeader("Authorization", headerData.toLocal8Bit());
    }

    QNetworkReply*  pnr = mNam->get(request);
    Q_UNUSED(pnr)
}

void Downloader::downloadPOST(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "curl/7.58.0");
    //request.setRawHeader(QNetworkRequest::UserAgentHeader, "MarsLviv");
    //request.setRawHeader("Authorization", "token 332b5f9145df8ae30d511ae4c6a92f48491b068b");
    // token (above) or credentials (belowe)
    QString username = "MarsLviv";
    QString password = "220iopMS";
    QString credentials = username + ":" + password;
    QByteArray data = credentials.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray jsonString = "{\"scopes\":[\"repo\",\"user\"],\"note\":\"getting-startedQ\"}";
    //QByteArray jsonString = "{\"name\":\"blogQ\",\"auto_init\":true,\"private\":true,\"gitignore_template\":\"nanoc\"}";
    QByteArray postDataSize = QByteArray::number(jsonString.size());
    request.setHeader(QNetworkRequest::ContentLengthHeader, postDataSize);
    //qDebug() << "postDataSize():" << postDataSize;

    QNetworkReply*  pnr = mNam->post(request, jsonString);
    Q_UNUSED(pnr)
    //QNetworkReply*  pnr = m_pnam->get(request);
}

void Downloader::onFinished(QNetworkReply* pnr)
{   //qDebug() << "Downloader::onFinished";
    if (pnr->error() != QNetworkReply::NoError)
    {
        emit error("error");
    }
    else
    {
        //  extructing key:values from response
        const QByteArray& ba = pnr->readAll();
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(ba);
        if(jsonDoc.isNull() || !jsonDoc.isObject() || jsonDoc.isEmpty())
        {
            emit corrupted("corrupted");
            return;
        }
        const QJsonObject json = jsonDoc.object();
        QMap<QString, QString> respondMap;
        //qDebug() << "Downloader::onFinished::RESPOND <>";
        foreach(const QString& key, json.keys())
        {
                respondMap[key] = json.value(key).toString();
                //qDebug() << key << ":" << json.value(key).toString();
        }
        //qDebug() << "Downloader::onFinished::RESPOND </>";

        //  extructing headers from response
        //qDebug() << "Downloader::onFinished::HEADERS <>";
        QList<QByteArray> headerList = pnr->rawHeaderList();
        QMap<QString, QString> headersMap;
        foreach(QByteArray head, headerList)
        {
            headersMap[head] = pnr->rawHeader(head);
            //qDebug() << head << ":" << pnr->rawHeader(head);
        }
        //qDebug() << "Downloader::onFinished::HEADERS </>";

        emit respond(pnr->url(), headersMap, respondMap);
    }
    pnr->deleteLater();
}
