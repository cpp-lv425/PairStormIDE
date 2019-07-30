#ifndef DOCUMENTATIONSEARCH_H
#define DOCUMENTATIONSEARCH_H

#include <QVector>
#include <QFileInfo>

class DocumentationSearch
{
public:
    static QVector<QFileInfo> search(const QDir &directory,const QString &keyword);
    static QVector<QFileInfo> search(const QDir &directory,const QString &include, const QString &function);
};

#endif // DOCUMENTATIONSEARCH_H
