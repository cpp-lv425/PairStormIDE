#include "highlightercpp.h"
#include <QDebug>


Highlightercpp::Highlightercpp(QTextDocument* parent):QSyntaxHighlighter (parent)

{

}


void Highlightercpp::setData(QVector<Token> _data)
{
    data = _data;
}

/*void Highlightercpp::setCurrentLine(unsigned int _line)
{
    currentLine = _line;
}*/

void Highlightercpp::highlightBlock(const QString &text)
{

}
