#ifndef STRUCTSFORDB_H
#define STRUCTSFORDB_H
#include <QString>
#include <QVector>

struct Message
{
    Message() = default;
    explicit Message(QString body, QString user, QString time): mBody{body}, mUser{user}, mTime{time}
    {
    }

    QString mBody;
    QString mUser;
    QString mTime;  //YYYY-MM-DD hh:mm:ss
};

struct User
{
    User() = default;
    explicit User(QString nickname): mNickname{nickname}
    {
    }

    QString mNickname;
};

struct File
{
    File() = default;
    explicit File(QString name): mName{name}
    {
    }

    QString mName;
};

struct Comment
{
    Comment() = default;
    explicit Comment(int line, QString text, QString user, QString file): mLine{line}, mText{text}, mUser{user}, mFile{file}
    {
    }

    int mLine;
    QString mText;
    QString mUser;
    QString mFile;
};

#endif // STRUCTSFORDB_H
