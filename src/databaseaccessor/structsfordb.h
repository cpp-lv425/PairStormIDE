#ifndef STRUCTSFORDB_H
#define STRUCTSFORDB_H
#include <QString>
struct Message
{
    QString mBody;
    int mIdUser;
    QString time;
};

struct User
{
    QString mNickname;
};

struct File
{
    QString mName;
};

struct Comment
{
    int mIdFile;
    int mLine;
    int mIdUser;
    QString mText;
};

#endif // STRUCTSFORDB_H
