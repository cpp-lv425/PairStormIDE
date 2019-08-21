#ifndef COMMENTDB_H
#define COMMENTDB_H
#include "accessor.h"
#include "structsfordb.h"

class CommentDb : public Accessor
{
public:
    CommentDb();
    void addCommentToDb(const Comment& comment);
    void deleteCommentFromDb(const int commentLine, const QString commentFile);
    void updateCommentInDb(const Comment& comment);
    QVector <Comment> getAllCommentsFromFile(const QString filename);
    Comment getCommentFromDb(const int commentLine, const QString commentFile);
private:
    QString addCommentQuery(const Comment& comment);
    QString deleteCommentQuery(const int commentLine, const QString commentFile);
    QString updateCommentQuery(const Comment& comment);
    QString getCommentQuery(const int commentLine, const QString commentFile);
    void fillStructComment(Comment &comment);
    QString numberOfCommentInFileQuery(const QString filename);
    QString allCommentInFileQuery(const QString filename);
};

#endif // COMMENTDB_H
