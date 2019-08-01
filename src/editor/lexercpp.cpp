#include "lexercpp.h"
#include <QDebug>

LexerCPP::LexerCPP()
{

}

LexerCPP::~LexerCPP()
{

}

bool LexerCPP::isKeyword(QString lexem)
{
    return KEYWORDS.contains(lexem);
}

bool LexerCPP::isIdentifier(QString lexem)
{
    QRegExp rx("[A-Za-z_][A-Za-z0-9_]*");
    return rx.exactMatch(lexem);
}

bool LexerCPP::isNumber(QString lexem)
{
    QRegExp rx("[0-9]+");
    return rx.exactMatch(lexem);
}

bool LexerCPP::isFloatNumber(QString lexem)
{
    QRegExp rx("[0-9]+\\.[0-9]+");
    return rx.exactMatch(lexem);
}

bool LexerCPP::isOperator(QString lexem)
{
    return OPERATORS.contains(lexem);
}

bool LexerCPP::isSymbolLiteral(QString lexem)
{
    QRegExp rx("'.'|'\.+'");
    return rx.exactMatch(lexem);
}

bool LexerCPP::isStringLiteral(QString lexem)
{
    QRegExp rx("\".*\"");
    return rx.exactMatch(lexem);
}

bool LexerCPP::isOneLineComment(QString lexem)
{
    QRegExp rx("//.*");
    return rx.exactMatch(lexem);
}

bool LexerCPP::isBlockComments(QString lexem)
{
    QRegExp rx("/\\*[\\s\\S]*\\*/");
    return rx.exactMatch(lexem);
}


bool LexerCPP::isSpace(QChar sym)
{
    return SPACES.contains(sym);
}

bool LexerCPP::isAlpha(QChar sym)

{
    QRegExp rx("[A-Za-z]");
    return rx.exactMatch(QString(sym));
}

bool LexerCPP::isDigit(QChar sym)
{
    QRegExp rx("[0-9]");
    return rx.exactMatch(QString(sym));
}

void LexerCPP::addLexem()
{
    --it;
    tokens.append(Token(QString(current_lexem), States(state), it - symbolCount - current_lexem.size(), it - symbolCount, current_line));
    current_lexem.clear();
    state = ST;
}


void LexerCPP::changeState(States st, QChar sym)
{
    state = st;
    current_lexem += sym;
}

void LexerCPP::clear()
{
    tokens.clear();
    current_lexem.clear();
    state = ST;
    current_line = 0;
}

void LexerCPP::lexicalAnalysis(QString code)
{
    it = 0;
    QChar sym = 0;
    while(code.size() - it)
    {
        sym = code[it];
        ++it;
        switch(state)
        {
            case ST:
                if(isSpace(sym)) break;
                if(isAlpha(sym))
                    changeState(ID, sym);
                else if(isDigit(sym))
                    changeState(NUM, sym);
                else if(isOperator(QString(sym)))
                    changeState(OPER, sym);
                else if(sym == '\'' || sym == '"')
                    changeState(LIT, sym);
            break;

            case ID:
                if(isSpace(sym) || isOperator((QString)sym) || sym == '\"' || sym == '\'')
                    addLexem();
                else if(isIdentifier(current_lexem + sym))
                {
                    current_lexem += sym;
                    if(isKeyword(current_lexem))
                        state = KW;
                }
                else
                    state = UNDEF;
            break;

            case KW:
                if(isSpace(sym) || isOperator((QString)sym) || sym == '\"' || sym == '\'')
                    addLexem();
                else if(isKeyword(current_lexem + sym))
                    current_lexem += sym;
                else if (isIdentifier(current_lexem + sym))
                    changeState(ID, sym);
                else
                    state = UNDEF;
            break;

            case NUM:
                if(sym == '.')
                    changeState(FNUM, sym);
                else if(isSpace(sym) || isOperator((QString)sym) || sym == '\"' || sym == '\'')
                    addLexem();
                else if(isNumber(current_lexem + sym))
                    current_lexem += sym;
                else
                    state = UNDEF;
            break;

            case FNUM:
                if(isSpace(sym) || isOperator((QString)sym) || sym == '\"' || sym == '\'')
                    addLexem();
                else if(isFloatNumber(current_lexem + sym))
                    current_lexem += sym;
                else
                    state = UNDEF;
            break;

            case OPER:
                if(isOneLineComment(current_lexem + sym) || (current_lexem + sym) == "/*")
                    changeState(COM, sym);
                else if(isSpace(sym) || !isOperator(current_lexem + sym) || sym == '\"' || sym == '\'')
                    addLexem();
                else if(isOperator(current_lexem + sym))
                    current_lexem += sym;
                else
                    state = UNDEF;
            break;

            case COM:
                if((isOneLineComment(current_lexem) && sym == '\n') ||
                    isBlockComments(current_lexem))
                    addLexem();
                else
                    current_lexem += sym;
            break;

            case LIT:
                if(isSymbolLiteral(current_lexem) || isStringLiteral(current_lexem))
                    addLexem();
                else if(sym == '\n')
                    addLexem();
                else
                    current_lexem += sym;
            break;

            case UNDEF:
                addLexem();
            break;
        }
    }
}
