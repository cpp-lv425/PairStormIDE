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

bool LexerCPP::isSpace(char sym)
{
    return SPACES.contains(sym);
}

bool LexerCPP::isAlpha(char sym)
{
    QRegExp rx("[A-Za-z]");
    return rx.exactMatch(QString(sym));
}

bool LexerCPP::isDigit(char sym)
{
    QRegExp rx("[0-9]");
    return rx.exactMatch(QString(sym));
}

void LexerCPP::addLexem()
{
    stream.seek(stream.pos() - 1);
    tokens.append(Token(QString(current_lexem), Type(state), stream.pos() - current_lexem.size(), stream.pos()));
    current_lexem.clear();
    state = ST;
}

void LexerCPP::changeState(States st, char sym)
{
    state = st;
    current_lexem += sym;
}

void LexerCPP::lexicalAnalysis(QString code)
{
    stream.setString(&code);
    char sym;

    while(!stream.atEnd())
    {

        stream >> sym;
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
