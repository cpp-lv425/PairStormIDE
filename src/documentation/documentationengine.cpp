#include "documentationengine.h"
#include "documentationsearch.h"

#include <QDir>

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
        auto temp = DocumentationSearch::search(documentation,keyword);
        foreach(const auto &a, temp)
        {
            mDocumentationLinks.push_back(a);
        }
    }

}

void DocumentationEngine::searchByLibraryAndFunction(const QString &include, const QString &function)
{
    mDocumentationLinks.clear();
    bool result{false};
    QDir documentation{DocumentationSearch::documentationDirectory(result)};
    if(result)
    {
        auto temp = DocumentationSearch::search(documentation,include,function);
        foreach(const auto &a, temp)
        {
            mDocumentationLinks.push_back(a);
        }
    }
}

QVector<QFileInfo> DocumentationEngine::documentationLinks() const
{
    return mDocumentationLinks;
}
