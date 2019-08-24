#include "downloader.h"
#include "downloaderWrapper.h"

#include <QNetworkConfigurationManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QUrl>
#include <QDebug>

DownloaderWrapper::DownloaderWrapper(QWidget* pwgt) : QWidget(pwgt)
{   //qDebug() << "DownloaderGui";
    mpDownloader  = new Downloader(this);
    connect(mpDownloader, &Downloader::respond, this,  &DownloaderWrapper::onRespond );
    connect(mpDownloader, &Downloader::error, this,  &DownloaderWrapper::respond );
    connect(mpDownloader, &Downloader::corrupted, this,  &DownloaderWrapper::respond );
    //qDebug() << "/DownloaderGui";
}

void DownloaderWrapper::get(const QString &userName, const QString &url,
                        const QString &tokenOrPassword)
{   //qDebug() << "DownloaderGui::get";
    mpDownloader->downloadGET(QUrl(url), userName, tokenOrPassword);
}

void DownloaderWrapper::post()
{
    mpDownloader->downloadPOST(QUrl(strPostLink));
}

void DownloaderWrapper::post2(const QString &userName, const QUrl &url, const QString &tokenOrPassword, const QByteArray &data)
{
    mpDownloader->downloadPOST2(QUrl(url), userName, tokenOrPassword, data);
}

void DownloaderWrapper::onRespond(const QUrl &url, const QMap<QString, QString> &headersMap, const QMap<QString, QString> &respondMap)
{
    mRespondUrl = url.toEncoded();
    mHeadersMap = headersMap;
    mRespondMap = respondMap;
    emit respond("normal");
}

