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
    unsigned int mCurrentLine;
    QString mCurrentLexem;

public:
    iLexer() = default;
    virtual ~iLexer() = default;
    virtual void lexicalAnalysis(QTextDocument*, int begin = -1, int end = -1) = 0;
};

#endif // ILEXER_H
