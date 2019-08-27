#include "filedb.h"

FileDb::FileDb(): Accessor ()
{
}

FileDb::~FileDb()
{
    query.finish();
}

void FileDb::addFileToDb(const File &file)
{
    execQuery(addFileQuery(file));
    query.finish();
}

File FileDb::getFileFromDb(const int idFile)
{
    execQuery(getFileQuery(idFile));
    File rFile;
    fillStructureFile(rFile);
    query.finish();
    return rFile;
}

void FileDb::deleteFileFromDb(const QString filename)
{
    execQuery(deleteFileQuery(filename));
    query.finish();
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

QString FileDb::deleteFileQuery(const QString filename)
{
    return "DELETE FROM User WHERE name = '" + filename + "'";
}

void FileDb::fillStructureFile(File &file)
{
    file.mName = query.record().value(0).toString();
}
