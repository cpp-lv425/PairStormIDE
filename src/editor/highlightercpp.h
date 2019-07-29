#ifndef HIGHLIGHTERCPP_H
#define HIGHLIGHTERCPP_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <token.h>

#include "lexercpp.h"
class Highlightercpp: QSyntaxHighlighter
{
private:
    unsigned int currentLine;
    QVector<Token> data;
public:

    Highlightercpp(QTextDocument* parent);
    void setData(QVector<Token>);
    void setCurrentLine(unsigned int line);
    void highlightBlock(const QString &text);

};

#endif // HIGHLIGHTERCPP_H
