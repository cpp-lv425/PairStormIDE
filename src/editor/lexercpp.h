#ifndef LEXERCPP_H
#define LEXERCPP_H
#include "ilexer.h"
#include "keywords.h"
#include "spaces.h"
#include "operators.h"


class LexerCPP: public iLexer
{
public:
    LexerCPP();
    ~LexerCPP() override;
    void addLexem();
    void lexicalAnalysis(QString) override;
    void changeState(States, char);
    bool isKeyword(QString);
    bool isIdentifier(QString);
    bool isNumber(QString);
    bool isFloatNumber(QString);
    bool isOperator(QString);
    bool isSymbolLiteral(QString);
    bool isStringLiteral(QString);
    bool isOneLineComment(QString);
    bool isBlockComments(QString);
    bool isAlpha(char);
    bool isDigit(char);
    bool isSpace(char);
};

#endif // LEXERCPP_H
