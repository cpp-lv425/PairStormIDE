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
    bool operator== (const Token argument)
    {
        return mName == argument.mName
               && mType == argument.mType
               && mBegin == argument.mBegin
               && mEnd == argument.mEnd;
    }
};

#endif // TOKEN_H
