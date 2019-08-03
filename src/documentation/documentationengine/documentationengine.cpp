#include "documentationengine.h"

#include <QDir>

#include "documentationsearch.h"

DocumentationEngine::DocumentationEngine(QObject *parent)
    : QObject(parent)
{

}

void DocumentationEngine::searchByKeyword(const QString &keyword)
{
    mDocumentationFiles.clear();
    bool result{false};
    QDir documentation{DocumentationSearch::documentationDirectory(result)};
    if(result)
    {
        mDocumentationFiles = DocumentationSearch::search(documentation,keyword);
    }
}

void DocumentationEngine::searchByLibraryAndFunction(const QString &include, const QString &function)
{
    mDocumentationFiles.clear();
    bool result{false};
    QDir documentation{DocumentationSearch::documentationDirectory(result)};
    if(result)
    {
        mDocumentationFiles = DocumentationSearch::search(documentation,include,function);
    }
}

QVector<QFileInfo> DocumentationEngine::documentationFiles() const
{
    return mDocumentationFiles;
}
