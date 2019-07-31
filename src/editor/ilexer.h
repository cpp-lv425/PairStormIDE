#ifndef ILEXER_H
#define ILEXER_H
#include <fmstates.h>
#include <token.h>
#include <QVector>

class iLexer
{
protected:
    QVector<Token> tokens;
    States state;
    int it;
    QString current_lexem;
    unsigned int current_line;
    unsigned int symbolCount;

public:
    iLexer();
    virtual ~iLexer() = 0;
    virtual void lexicalAnalysis(QString) = 0;
    QVector<Token> getTokens();
};

#endif // ILEXER_H
