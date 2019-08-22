#ifndef COMMENTDB_H
#define COMMENTDB_H
#include "accessor.h"
#include "structsfordb.h"

class CommentDb : public Accessor
{
public:
    CommentDb();
    void addCommentsToDb(const QVector<Comment> &comments);
    void deleteCommentFromDb(const int commentLine, const QString commentFile);
    void deleteCommentsFromDb(const QString commentFile);
    QVector <Comment> getAllCommentsFromFile(const QString filename);
    Comment getCommentFromDb(const int commentLine, const QString commentFile);
private:
    QString addCommentQuery(const Comment& comment);
    QString deleteCommentQuery(const int commentLine, const QString commentFile);
    QString getCommentQuery(const int commentLine, const QString commentFile);
    void fillStructComment(Comment &comment);
    QString numberOfCommentInFileQuery(const QString filename);
    QString allCommentInFileQuery(const QString filename);
};

#endif // COMMENTDB_H
