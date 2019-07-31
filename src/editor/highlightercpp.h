#ifndef HIGHLIGHTERCPP_H
#define HIGHLIGHTERCPP_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <token.h>

#include "lexercpp.h"
class Highlightercpp: QSyntaxHighlighter
{
private:
    int mCurrentLine;
    QVector<Token> mData;
    QString mTextEditor;
    QTextCharFormat mFormatKeyword;
    QTextCharFormat mFormatLiteral;
    QTextCharFormat mFormatComment;

public:
    QStringList mLines;
    Highlightercpp(QTextDocument* parent);
    void setData(QVector<Token>);
    void findLine(QString text);
    void highlightBlock(const QString &text);
    void setText(QString _textEditor);
};

#endif // HIGHLIGHTERCPP_H
