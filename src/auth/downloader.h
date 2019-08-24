#pragma once

#include <QObject>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QUrl;
QT_END_NAMESPACE

class Downloader : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager *mNam;

public:
    Downloader(QObject* pobj = nullptr);

    void download(const QUrl&);
    void downloadGET(const QUrl &url, const QString &userName,
                     const QString &tokenOrPassword = "");
    void downloadPOST(const QUrl&);
    void downloadPOST2(const QUrl &url,                const QString &userName,
                       const QString &tokenOrPassword, const QByteArray &data);

signals:
    void respond         (const QUrl &url, const QMap<QString, QString> &headersMap, const QMap<QString, QString> &respondMap);
    void error           (const QString &message);
    void corrupted       (const QString &message);

private slots:
    void onFinished(QNetworkReply*);
};

