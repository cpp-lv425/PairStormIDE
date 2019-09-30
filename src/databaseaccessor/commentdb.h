#ifndef COMMENTDB_H
#define COMMENTDB_H
#include "accessor.h"
#include "structsfordb.h"

class CommentDb : public Accessor
{
public:
    CommentDb();
    ~CommentDb();
    void addCommentsToDb(const QVector<Comment> &comments);
    void deleteCommentsFromDb(const QString& commentFile);
    QVector <Comment> getAllCommentsFromFile(const QString filename);
private:
    static QString addCommentQuery(const Comment& comment);
    static QString deleteCommentQuery(const int commentLine, const QString commentFile);
    static QString getCommentQuery(const int commentLine, const QString commentFile);
    void fillStructComment(Comment &comment);
    static QString numberOfCommentInFileQuery(const QString filename);
    static QString allCommentInFileQuery(const QString filename);
    static QString deleteAllCommentsInFileQuery(const QString commentFile);
};

#endif // COMMENTDB_H
