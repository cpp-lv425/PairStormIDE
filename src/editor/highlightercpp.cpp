#include "highlightercpp.h"
#include <QDebug>


Highlightercpp::Highlightercpp(QTextDocument* parent):QSyntaxHighlighter (parent) 
{
    //font color settings
    formatkeyword.setForeground(QColor(0, 102, 255));
    formatliteral.setForeground(QColor(153, 0, 255));
    formatcomment.setForeground(QColor(0, 255, 204));
}


void Highlightercpp::setData(QVector<Token> _data)
{
    data = _data;
}

void Highlightercpp::findLine(QString text)
{
    if(text.size()==0) currentLine = -1;
    for(unsigned int i = 0; i < lines.size(); i++)
    {
        int y = QString::compare(lines[i], text);
        if(y == 0)
        {
            currentLine = i;
            return;
        }
    }
}

void Highlightercpp::setText (QString _textEditor)
{
    textEditor = _textEditor;
    lines = textEditor.split("\n");
}

void Highlightercpp::highlightBlock(const QString &text)
{
    findLine(text);

    for(auto p: data)
    {
        if(currentLine == p.linesCount)
        {
            if(p.type == KW)
            {
                setFormat(p.begin, p.end - p.begin , formatkeyword);
            }
            else if(p.type == LIT)
            {
                setFormat(p.begin, p.end - p.begin , formatliteral);
            }
            else if(p.type == COM)
            {
                setFormat(p.begin, p.end - p.begin, formatcomment);
            }
        }
    }


}
