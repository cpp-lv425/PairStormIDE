#ifndef COMMENTDB_H
#define COMMENTDB_H
#include "accessor.h"
#include "structsfordb.h"

class CommentDb : public Accessor
{
public:
    CommentDb();
    void addCommentToDb(const dbComment& comment);
    void deleteCommentFromDb(const int commentLine, const int commentFile);
    void updateCommentInDb(const dbComment& comment);
    void getCommentFromDb(const int commentLine, const int commentFile);
private:
    QString addCommentQuery(const dbComment& comment);
    QString deleteCommentQuery(const int commentLine, const int commentFile);
    QString updateCommentQuery(const dbComment& comment);
    QString getCommentQuery(const int commentLine, const int commentFile);
};

#endif // COMMENTDB_H
