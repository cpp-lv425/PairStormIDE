#include "highlightercpp.h"
#include <QDebug>


Highlightercpp::Highlightercpp(QTextDocument* parent):QSyntaxHighlighter (parent) 
{
    mCurrentLine = 0;
    //font color settings
    mFormatKeyword.setForeground(QColor(0, 102, 255));
    mFormatLiteral.setForeground(QColor(153, 0, 255));
    mFormatComment.setForeground(QColor(0, 255, 204));
}

void Highlightercpp::setData(QVector<Token> _mData)
{
    mData = _mData;
}

void Highlightercpp::findLine(QString text)
{
    if(text.size() == 0)
        mCurrentLine = -1;

    for(unsigned int i = 0; i < mLines.size(); i++)
    {
        int y = QString::compare(mLines[i], text);
        if(y == 0)
        {
            mCurrentLine = i;
            return;
        }
    }
}

void Highlightercpp::setText(QString TextEditor)
{
    mTextEditor = TextEditor;
    mLines = mTextEditor.split("\n");
}

void Highlightercpp::highlightBlock(const QString &text)
{
    qDebug() << ":3\n";
}
