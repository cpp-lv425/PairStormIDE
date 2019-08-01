#ifndef TOKEN_H
#define TOKEN_H
#include <QString>

#include <fmstates.h>

struct Token {
    QString name;
    States type;
    unsigned int begin;
    unsigned int end;
    Token(QString _name, States _type, unsigned int _begin, unsigned int _end, unsigned int _linesCount):
          name(_name), type(_type), begin(_begin), end(_end) {}
};

#endif // TOKEN_H
