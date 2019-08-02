#include "documentationengine.h"

#include <QDir>

#include "documentationsearch.h"

DocumentationEngine::DocumentationEngine(QObject *parent)
    : QObject(parent)
{

}

void DocumentationEngine::searchByKeyword(const QString &keyword)
{
    mDocumentationLinks.clear();
    bool result{false};
    QDir documentation{DocumentationSearch::documentationDirectory(result)};
    if(result)
    {
        mDocumentationLinks = DocumentationSearch::search(documentation,keyword);
    }

}

void DocumentationEngine::searchByLibraryAndFunction(const QString &include, const QString &function)
{
    mDocumentationLinks.clear();
    bool result{false};
    QDir documentation{DocumentationSearch::documentationDirectory(result)};
    if(result)
    {
        mDocumentationLinks = DocumentationSearch::search(documentation,include,function);
    }
}

QVector<QFileInfo> DocumentationEngine::documentationLinks() const
{
    return mDocumentationLinks;
}
