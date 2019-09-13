#include "downloader.h"
#include "downloaderWrapper.h"

#include <QNetworkConfigurationManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>

DownloaderWrapper::DownloaderWrapper(QObject *pwgt) : QObject(pwgt)
{
    mpDownloader  = new Downloader(this);
    connect(mpDownloader, &Downloader::respond, this,  &DownloaderWrapper::onRespond );
    connect(mpDownloader, &Downloader::error, this,  &DownloaderWrapper::respond ); // resend signal about error to outer object
    connect(mpDownloader, &Downloader::corrupted, this,  &DownloaderWrapper::respond );// resend signal about data corruption to outer object
}

void DownloaderWrapper::get(const QString &userName,
                            const QString &url,
                            const QString &tokenOrPassword)
{
    mpDownloader->downloadGET(QUrl(url), userName, tokenOrPassword);
}

void DownloaderWrapper::post(const QString &userName,
                             const QString &url,
                             const QString &tokenOrPassword,
                             const QByteArray &data)
{
    mpDownloader->downloadPOST(QUrl(url), userName, tokenOrPassword, data);
}

void DownloaderWrapper::onRespond(const QUrl &url,
                                  const QMap<QString, QString> &headersMap,
                                  const QMap<QString, QString> &respondMap)
{
    mRespondUrl = url.toEncoded();
    mHeadersMap = headersMap;
    mRespondMap = respondMap;
    emit respond("normal");
}


