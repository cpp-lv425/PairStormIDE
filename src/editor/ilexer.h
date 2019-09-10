#ifndef ILEXER_H
#define ILEXER_H

#include <QTextDocument>
#include <QMap>
#include <QVector>

class iLexer
{
protected:
    unsigned int mIndex;
    unsigned int mCodeSize;
    unsigned int mLinesCount;
    unsigned int mCurrentLine;
    QString mCurrentLexem;

public:
    iLexer()
    {
        mLinesCount = 1;
    }
    virtual ~iLexer() = default;
    virtual void lexicalAnalysis(QString) = 0;
};

#endif // ILEXER_H
