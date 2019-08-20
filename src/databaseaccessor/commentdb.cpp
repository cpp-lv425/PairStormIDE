#include "commentdb.h"

CommentDb::CommentDb(): Accessor()
{
}

void CommentDb::addCommentToDb(const Comment &comment)
{
    execQuery(addCommentQuery(comment));
}

void CommentDb::deleteCommentFromDb(const int commentLine, const QString commentFile)
{
    execQuery(deleteCommentQuery(commentLine, commentFile));
}

void CommentDb::updateCommentInDb(const Comment& comment)
{
    execQuery(updateCommentQuery(comment));
}

QVector<Comment> CommentDb::getAllCommentsFromDB(const QString filename)
{
      query.first();
      execQuery(numberOfCommentInFileQuery(filename));
      unsigned int count_of_messages =query.value(0).toUInt();
      QVector<Comment> comments(count_of_messages);
      int counter = 0;
      while (query.next()) {
          fillStructComment(comments[counter]);
          counter++;
      }
      return comments;
}

void CommentDb::getCommentFromDb(const int commentLine, const QString commentFile)
{
    execQuery(getCommentQuery(commentLine, commentFile));
    query.record();
}

QString CommentDb::addCommentQuery(const Comment &comment)
{
    return "INSERT INTO Comment (line, idFile, idUser, text) VALUES ('"
            +QString::number(comment.mLine) + "', "
            "(SELECT id FROM File WHERE name = '" + comment.mIdFile + "'), "
            "(SELECT id FROM User WHERE nickname = '" + comment.mIdUser + "'), '"
            +comment.mText + "')" ;
}

QString CommentDb::deleteCommentQuery(const int commentLine, const QString commentFile)
{
    return "DELETE FROM Comment WHERE idFile ="
            "(SELECT id FROM File where name = "
            + commentFile
            +") AND line = " +
            QString::number(commentLine);
}

QString CommentDb::updateCommentQuery(const Comment& comment)
{
    return "UPDATE Comment SET idUser = (SELECT id FROM User WHERE nickname = '" + comment.mIdUser + "'), "
            +"text = " + comment.mText
            + " WHERE line = " + QString::number(comment.mLine)
            +" AND idFile = (SELECT id FROM File WHERE name = '" + comment.mIdFile + "')";
}

QString CommentDb::getCommentQuery(const int commentLine, const QString commentFile)
{
    return "SELECT * FROM Comment WHERE idFile = "
           "(SELECT id FROM File where name = "
            + commentFile
            + ") AND line = " +
            QString::number(commentLine);
}

void CommentDb::fillStructComment(Comment &comment)
{
    comment.mLine = query.record().value(0).toInt();
    comment.mText = query.record().value(3).toString();
    //etc.
}

QString CommentDb::numberOfCommentInFileQuery(const QString filename)
{
    return "SELECT COUNT(line) FROM Comment WHERE idFile = '"
            + filename + "'";
}
