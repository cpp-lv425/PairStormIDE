#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include "fmstates.h"

struct Token
{
    QString mName;
    State mType;
    int mBegin;
    int mEnd;
    Token(QString name, State type, unsigned int begin, unsigned int end):
          mName(name), mType(type), mBegin(begin), mEnd(end) {}
};

#endif // TOKEN_H
