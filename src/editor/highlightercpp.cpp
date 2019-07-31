#include "highlightercpp.h"
#include <QDebug>

Highlightercpp::Highlightercpp(QTextDocument* parent):QSyntaxHighlighter (parent)
{

}

void Highlightercpp::setData(QVector<Token> _data)
{
    data = _data;
}

void Highlightercpp::setCurrentLine(unsigned int _line)
{
    currentLine = _line;
}

void Highlightercpp::highlightBlock(const QString &text)
{
    QTextCharFormat kw;
    kw.setForeground(Qt::red);
    for(auto it = data.begin(); it < data.end(); ++it)
    {
        qDebug() << it->name << " " << it->type << " " << it->begin << " " << it->end << " " << it->linesCount <<'\n';
        setFormat(0, text.size(), QTextCharFormat());
        if(it->type == KW && it->linesCount == currentLine)
        {
            setFormat(it->begin, it->end, kw);
        }
    }
    qDebug() << "*******************************";
}
