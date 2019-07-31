#include "documentationengine.h"
#include "documentationsearch.h"

#include <QDir>

DocumentationWebEngine::DocumentationWebEngine(QObject *parent)
    : QObject(parent)
{

}

void DocumentationWebEngine::searchByKeyword(const QString &keyword)
{
    mDocumentationLinks.clear();
    bool result{false};
    QDir documentation{DocumentationSearch::documentationDirectory(result)};
    if(result)
    {
        auto temp = DocumentationSearch::search(documentation,keyword);
        foreach(const auto &a, temp)
        {
            mDocumentationLinks.push_back(DocumentationSearch::getUrl(a));
        }
    }
    else
    {
        mDocumentationLinks.push_back(QUrl("https://en.cppreference.com/w/"));
    }
}

void DocumentationWebEngine::searchByLibraryAndFunction(const QString &include, const QString &function)
{
    mDocumentationLinks.clear();
    bool result{false};
    QDir documentation{DocumentationSearch::documentationDirectory(result)};
    if(result)
    {
        auto temp = DocumentationSearch::search(documentation,include,function);
        foreach(const auto &a, temp)
        {
            mDocumentationLinks.push_back(DocumentationSearch::getUrl(a));
        }
    }
    else
    {
        mDocumentationLinks.push_back(QUrl("https://en.cppreference.com/w/"));
    }
}

QVector<QUrl> DocumentationWebEngine::documentationLinks() const
{
    return mDocumentationLinks;
}
