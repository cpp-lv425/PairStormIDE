#ifndef HIGHLIGHTERCPP_H
#define HIGHLIGHTERCPP_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <token.h>

#include "lexercpp.h"
class Highlightercpp: QSyntaxHighlighter
{
private:
    int currentLine=0;
    QVector<Token> data;
    QString textEditor;

    QTextCharFormat formatkeyword;
    QTextCharFormat formatliteral;
    QTextCharFormat formatcomment;

public:
    QStringList lines;
    Highlightercpp(QTextDocument* parent);
    void setData(QVector<Token>);
    void findLine(QString text);
    void highlightBlock(const QString &text);
    void setText(QString _textEditor);
};

#endif // HIGHLIGHTERCPP_H
