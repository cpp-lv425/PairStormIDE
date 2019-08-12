#ifndef TEXTDOCUMENTHOLDER_H
#define TEXTDOCUMENTHOLDER_H

#include <QPlainTextEdit>

class TextDocumentHolder: public QPlainTextEdit
{
    QString mFileName;
    size_t mViewCount;
public:
    TextDocumentHolder(const QString &fileName, QWidget *pParent = nullptr);
    const QString& fileName() const;
    void setFileName(const QString &fileName);
    void incrementViewCount();
};

#endif // TEXTDOCUMENTHOLDER_H
