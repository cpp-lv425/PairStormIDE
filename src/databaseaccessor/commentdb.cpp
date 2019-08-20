#include "commentdb.h"

CommentDb::CommentDb(): Accessor()
{
}

void CommentDb::addCommentToDb(const Comment &comment)
{
    execQuery(addCommentQuery(comment));
}

void CommentDb::deleteCommentFromDb(const int commentLine, const int commentFile)
{
    execQuery(deleteCommentQuery(commentLine, commentFile));
}

void CommentDb::updateCommentInDb(const Comment& comment)
{
    execQuery(updateCommentQuery(comment));
}

void CommentDb::getCommentFromDb(const int commentLine, const int commentFile)
{
    execQuery(getCommentQuery(commentLine, commentFile));
}

QString CommentDb::addCommentQuery(const Comment &comment)
{
    return "INSERT INTO Comment (line, idFile, idUser, text) VALUES ('"
            +QString::number(comment.mLine) + "', '"
            +QString::number(comment.mIdFile) + "', '"
            +QString::number(comment.mIdUser) + "', '"
            +comment.mText + "')" ;
}

QString CommentDb::deleteCommentQuery(const int commentLine, const int commentFile)
{
    return "DELETE FROM Comment WHERE idFile = "
            + QString::number(commentFile)
            +" AND line = " +
            QString::number(commentLine);
}

QString CommentDb::updateCommentQuery(const Comment& comment)
{
    return "UPDATE Comment SET idUser = '" + QString::number(comment.mIdUser)
            +"', text = " + comment.mText
            + " WHERE line = " + QString::number(comment.mLine)
            +" AND idFile = " + QString::number(comment.mIdFile);
}

QString CommentDb::getCommentQuery(const int commentLine, const int commentFile)
{
    return "SELECT * FROM Comment WHERE idFile = "
            + QString::number(commentFile)
            +" AND " +
            QString::number(commentLine);
}
