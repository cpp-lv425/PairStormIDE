#include "fileDb.h"

FileDb::FileDb() :Accessor ()
{

}

void FileDb::addFileToDb(const dbFile &file)
{
    execQuery(addFileQuery(file));
}

void FileDb::getFileFromDb(const int idFile)
{
    execQuery(getFileQuery(idFile));
}

void FileDb::deleteFileFromDb(const QString filename)
{
    execQuery(deleteFileQuery(filename));
}

QString FileDb::addFileQuery(const dbFile &file)
{
    return "INSERT INTO File (name) VALUES ('"
            +file.mName + "')";
}

QString FileDb::getFileQuery(const int idFile)
{
    return "SELECT name from File WHERE id = "
            + QString::number(idFile);
}

QString FileDb::deleteFileQuery(const QString filename)
{
    return "DELETE FROM User WHERE name = '" + filename + "'";
}
