#ifndef STRUCTSFORDB_H
#define STRUCTSFORDB_H
#include <QString>
#include <QVector>
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
    QString mIdFile;
    int mLine;
    QString mIdUser;
    QString mText;
};

#endif // STRUCTSFORDB_H
