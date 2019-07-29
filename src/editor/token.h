#ifndef TOKEN_H
#define TOKEN_H
#include <QString>
#include <type.h>

struct Token {
    QString name;
    Type type;
    unsigned int begin;
    unsigned int end;
    Token(QString _name, Type _type, unsigned int _begin, unsigned int _end):
          name(_name), type(_type), begin(_begin), end(_end) {}
};

#endif // TOKEN_H
