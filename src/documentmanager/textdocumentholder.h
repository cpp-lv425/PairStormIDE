#ifndef TEXTDOCUMENTHOLDER_H
#define TEXTDOCUMENTHOLDER_H

#include <QPlainTextEdit>

class TextDocumentHolder: public QPlainTextEdit
{
    QString mFileName;
    size_t mViewCount;
public:
    TextDocumentHolder(const QString &getFileName, QWidget *pParent = nullptr);
    const QString& getFileName() const;
    void setFileName(const QString &getFileName);
    void incrementViewCount();
};

#endif // TEXTDOCUMENTHOLDER_H
