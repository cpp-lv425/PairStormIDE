#include "textdocumentholder.h"

TextDocumentHolder::TextDocumentHolder(const QString &fileName, QWidget *pParent):
    QPlainTextEdit (pParent),
    mFileName(fileName),
    mViewCount(0)
{
}

const QString& TextDocumentHolder::getFileName() const
{
    return mFileName;
}

void TextDocumentHolder::setFileName(const QString &fileName)
{
    mFileName = fileName;
}

void TextDocumentHolder::incrementViewCount()
{
    ++mViewCount;
}
