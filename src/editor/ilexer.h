#ifndef ILEXER_H
#define ILEXER_H

#include <QVector>

class iLexer
{
protected:
    unsigned int mIndex;
    QString mCurrentLexem;

public:
    iLexer() = default;
    virtual ~iLexer() = default;
    virtual void lexicalAnalysis(QString) = 0;
};

#endif // ILEXER_H
