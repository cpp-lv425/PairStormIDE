#include "documentationsearch.h"

#include <QDebug>
#include <QDirIterator>
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

QVector<QFileInfo> DocumentationSearch::search(const QDir &directory, const QString &include, const QString &keyword)
{
    QVector<QFileInfo> rResult;
    QDirIterator it(directory, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        if(it.fileInfo().suffix() == "html" && it.fileInfo().baseName()== keyword && it.fileInfo().dir().dirName() == include)
        {
             rResult.push_back(it.fileInfo());
        }
        it.next();
    }
    return rResult;
}
