#ifndef COMMENTDB_H
#define COMMENTDB_H
#include "accessor.h"
#include "structsfordb.h"

class CommentDb : public Accessor
{
public:
    CommentDb();
    void addCommentToDb(const Comment& comment);
    void deleteCommentFromDb(const int commentLine, const int commentFile);
    void updateCommentInDb(const Comment& comment);
    void getCommentFromDb(const int commentLine, const int commentFile);
private:
    QString addCommentQuery(const Comment& comment);
    QString deleteCommentQuery(const int commentLine, const int commentFile);
    QString updateCommentQuery(const Comment& comment);
    QString getCommentQuery(const int commentLine, const int commentFile);
};

#endif // COMMENTDB_H
