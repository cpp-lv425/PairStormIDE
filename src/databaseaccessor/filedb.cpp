#include "filedb.h"

FileDb::FileDb() :Accessor ()
{

}

void FileDb::addFileToDb(const File &file)
{
    execQuery(addFileQuery(file));
}

void FileDb::getFileFromDb(const int idFile)
{
    execQuery(getFileQuery(idFile));
}

void FileDb::getFileIdFromDb(const QString filename)
{
    execQuery(getFileIdQuery(filename));
}

void FileDb::deleteFileFromDb(const QString filename)
{
    execQuery(deleteFileQuery(filename));
}

QString FileDb::addFileQuery(const File &file)
{
    return "INSERT INTO File (name) VALUES ('"
            +file.mName + "')";
}

QString FileDb::getFileQuery(const int idFile)
{
    return "SELECT name from File WHERE ID = "
            + QString::number(idFile);
}

QString FileDb::getFileIdQuery(const QString filename)
{
    return "SELECT ID FROM File WHERE name = '"
            + filename + "'";
}

QString FileDb::deleteFileQuery(const QString filename)
{
    return "DELETE FROM User WHERE name = '" + filename + "'";
}
