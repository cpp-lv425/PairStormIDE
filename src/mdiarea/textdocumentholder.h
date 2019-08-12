#ifndef TEXTDOCUMENTHOLDER_H
#define TEXTDOCUMENTHOLDER_H

#include <QPlainTextEdit>

class TextDocumentHolder: public QPlainTextEdit
{
public:
    TextDocumentHolder(QWidget *pParent = nullptr);
};

#endif // TEXTDOCUMENTHOLDER_H
