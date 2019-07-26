#ifndef ILEXER_H
#define ILEXER_H
#include <fmstates.h>
#include <token.h>
#include <QVector>
#include <QTextStream>

class iLexer
{
private:
    QVector<Token> tokens;
    States state;
    QTextStream stream;
    QString current_lexem;
public:
    iLexer();
    virtual ~iLexer() = 0;
    virtual void lexicalAnalysis() = 0;
    QVector<Token> getTokens();
};

#endif // ILEXER_H
