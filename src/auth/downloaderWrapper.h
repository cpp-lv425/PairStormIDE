#pragma once

#include <QObject>
#include <QMap>

QT_BEGIN_NAMESPACE
class Downloader;
class QUrl;
QT_END_NAMESPACE

class DownloaderWrapper : public QObject
{
Q_OBJECT

private:
    Downloader *mpDownloader;

    QString mUserName;
    QString mPassword;
    QString mToken;
    QString mUrl;
    QByteArray mData;

public:
    DownloaderWrapper(QObject *pwgt = nullptr);

    void get( const QString &userName,
              const QString &url,
              const QString &tokenOrPassword = "");
    void post(const QString &userName,
              const QString &url,
              const QString &tokenOrPassword,
              const QByteArray &data);

    QString mRespondUrl;
    QMap<QString, QString> mRespondMap;
    QMap<QString, QString> mHeadersMap;

private slots:
    void onRespond(const QUrl &url,
                   const QMap<QString, QString> &headersMap,
                   const QMap<QString, QString> &respondMap);

signals:
    void respond(const QString &status);

};

