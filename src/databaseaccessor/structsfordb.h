#ifndef STRUCTSFORDB_H
#define STRUCTSFORDB_H
#include <QString>
struct dbMessage
{
    QString mBody;
    int mIdUser;
    QString time;
};

struct dbUser
{
    QString mNickname;
};

struct dbFile
{
    QString mName;
};

struct dbComment
{
    int mIdFile;
    int mLine;
    int mIdUser;
    QString mText;
};

#endif // STRUCTSFORDB_H
