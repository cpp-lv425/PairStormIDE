#ifndef LEXERCPP_H
#define LEXERCPP_H

#include "ilexer.h"
#include "fmstates.h"
#include "token.h"
#include "keywords.h"
#include "spaces.h"
#include "operators.h"
#include <QVector>
#include <QMap>
#include <QTextBlock>

class LexerCPP: public iLexer
{
private:
    QMap<int, QVector<Token>> mTokens;
    QVector<Token> mTokensOnCurrentLine;
    State mState;
    inline void addLexem();
    inline void changeState(State, QChar);
    inline bool isKeyword(const QString&);
    inline bool isIdentifier(const QString&);
    inline bool isNumber(const QString&);
    inline bool isFloatNumber(const QString&);
    inline bool isOperator(const QString&);
    inline bool isSymbolLiteral(const QString&);
    inline bool isStringLiteral(const QString&);
    inline bool isOneLineComment(const QString&);
    inline bool isBlockComments(const QString&);
    inline bool isLetter(const QChar&);
    inline bool isDigit(const QChar&);
    inline bool isSpace(const QChar&);
    inline bool isQuote(const QChar&);
    inline bool isLexemEnd(const QChar&);
    void handleStartState(const QChar&);
    void handleOperatorState(const QChar&);
    void handleKeywordState(const QChar&);
    void handleIdentifierState(const QChar&);
    void handleNumberState(const QChar&);
    void handleFloatNumberState(const QChar&);
    void handleLiteralState(const QChar&);
    void handleCommentState(const QChar&);
    void handleUndefinedState(const QChar&);

public:
    LexerCPP() = default;
    ~LexerCPP() override = default;
    void lexicalAnalysis(QString) override;
    QVector<Token> getTokens() const;
    void clear();
};

#endif // LEXERCPP_H
