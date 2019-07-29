#ifndef LEXERCPP_H
#define LEXERCPP_H
#include <QTextBlock>
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
    void changeState(States, QChar);
    bool isKeyword(QString);
    bool isIdentifier(QString);
    bool isNumber(QString);
    bool isFloatNumber(QString);
    bool isOperator(QString);
    bool isSymbolLiteral(QString);
    bool isStringLiteral(QString);
    bool isOneLineComment(QString);
    bool isBlockComments(QString);
    bool isAlpha(QChar);
    bool isDigit(QChar);
    bool isSpace(QChar);
    void clear();
};

#endif // LEXERCPP_H
