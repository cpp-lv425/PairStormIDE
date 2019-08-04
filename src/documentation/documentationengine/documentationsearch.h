#ifndef DOCUMENTATIONSEARCH_H
#define DOCUMENTATIONSEARCH_H

#include <QUrl>
#include <QVector>
#include <QFileInfo>

class DocumentationSearch
{
public:
    static QVector<QFileInfo> search(const QDir &directory, const QString &keyword);
    static QVector<QFileInfo> search(const QDir &directory, const QString &keyword, const QString &library);
    static QUrl getUrl(const QFileInfo &info);
    static QDir documentationDirectory(bool &result);
};

#endif // DOCUMENTATIONSEARCH_H
