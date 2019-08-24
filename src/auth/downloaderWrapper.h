#pragma once

//#include <QObject>
#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class Downloader;
class QUrl;
//class QMap;
QT_END_NAMESPACE

//class DownloaderGui : public QObject {
class DownloaderWrapper : public QWidget {
Q_OBJECT

private:
    Downloader*   mpDownloader;

    QString mUserName;
    QString mPassword;
    QString mToken;
    QString mUrl;
    QByteArray mData;

    QString strDownloadLink = "https://api.github.com/user";
    QString strPostLink = "https://api.github.com/user/repos";
public:
    //DownloaderWrapper(QObject* pwgt = nullptr);
    DownloaderWrapper(QWidget* pwgt = nullptr);

    void get(const QString &userName, const QString &url,
             const QString &tokenOrPassword = "");
    void post();

    QString mRespondUrl;
    QMap<QString, QString> mRespondMap;
    QMap<QString, QString> mHeadersMap;
private slots:
    void onRespond(const QUrl &url, const QMap<QString, QString> &headersMap, const QMap<QString, QString> &respondMap);
signals:
    void respond(const QString &status);
};

