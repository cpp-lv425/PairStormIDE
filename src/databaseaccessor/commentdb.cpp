#include "commentdb.h"

CommentDb::CommentDb(): Accessor()
{
}

void CommentDb::addCommentToDb(const Comment &comment)
{
    execQuery(addCommentQuery(comment));
    query.finish();
}

void CommentDb::deleteCommentFromDb(const int commentLine, const QString commentFile)
{
    execQuery(deleteCommentQuery(commentLine, commentFile));
    query.finish();
}

void CommentDb::updateCommentInDb(const Comment& comment)
{
    execQuery(updateCommentQuery(comment));
    query.finish();
}

QVector<Comment> CommentDb::getAllCommentsFromFile(const QString filename)
{
      execQuery(numberOfCommentInFileQuery(filename));
      int count_of_messages =query.value(0).toInt();
      QVector<Comment> comments(count_of_messages);
      execQuery(allCommentInFileQuery(filename));
      int counter = 0;
      while (query.next()) {
          fillStructComment(comments[counter]);
          counter++;
      }
      query.finish();
      return comments;
}

Comment CommentDb::getCommentFromDb(const int commentLine, const QString commentFile)
{
    execQuery(getCommentQuery(commentLine, commentFile));
    Comment rComment;
    fillStructComment(rComment);
    query.finish();
    return rComment;
}

QString CommentDb::addCommentQuery(const Comment &comment)
{
    return "INSERT INTO Comment (line, idFile, idUser, text) VALUES ('"
            +QString::number(comment.mLine) + "', "
            "(SELECT id FROM File WHERE name = '" + comment.mFile + "'), "
            "(SELECT id FROM User WHERE nickname = '" + comment.mUser + "'), '"
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
    return "UPDATE Comment SET idUser = (SELECT id FROM User WHERE nickname = '" + comment.mUser + "'), "
            +"text = " + comment.mText
            + " WHERE line = " + QString::number(comment.mLine)
            +" AND idFile = (SELECT id FROM File WHERE name = '" + comment.mFile + "')";
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
    comment.mText = query.record().value(1).toString();
    comment.mUser = query.record().value(2).toString();
    comment.mFile = query.record().value(3).toString();
}

QString CommentDb::numberOfCommentInFileQuery(const QString filename)
{
    return "Select Count(Comment.line) "
           "from Comment inner join User on User.id=Comment.idUser "
            "inner join File on File.id=Comment.idFile "
            "where Comment.idFile = (Select File.ID from File where file.name = \""
            + filename + "\"";
}

QString CommentDb::allCommentInFileQuery(const QString filename)
{
    return "Select Comment.line, Comment.text, User.nickname, File.name "
           "from Comment inner join User on User.id=Comment.idUser "
            "inner join File on File.id=Comment.idFile "
            "where Comment.idFile = (Select File.ID from File where file.name = \""
            + filename + "\"";
}
