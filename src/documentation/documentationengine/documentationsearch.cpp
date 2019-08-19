#include "documentationsearch.h"

#include <QDebug>
#include <QDirIterator>
#include <QStandardPaths>

QVector<QFileInfo> DocumentationSearch::search(const QDir &directory, const QString &keyword)
{
    QVector<QFileInfo> rResult;
    QDirIterator it(directory, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        if(it.fileInfo().suffix() == "html" && it.fileInfo().baseName()== keyword)
        {
             rResult.push_back(it.fileInfo());
        }
        it.next();
    }
    return rResult;
}

QVector<QFileInfo> DocumentationSearch::search(const QDir &directory, const QString &keyword, const QString &library)
{
    QVector<QFileInfo> rResult;
    QDirIterator it(directory, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        if(it.fileInfo().suffix() == "html" && it.fileInfo().baseName()== keyword && it.fileInfo().dir().dirName() == library)
        {
             rResult.push_back(it.fileInfo());
        }
        it.next();
    }
    return rResult;
}

QUrl DocumentationSearch::getUrl(const QFileInfo &info)
{
    return QUrl::fromLocalFile(info.filePath());
}

QDir DocumentationSearch::documentationDirectory(bool &result)
{
    QString documents(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    QDir documentation(documents);
    result = documentation.cd("PairStorm");
    result = documentation.cd("reference");
    return result ? documentation : QDir::home();
}
