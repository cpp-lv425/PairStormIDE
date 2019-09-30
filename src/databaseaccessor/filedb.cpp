#include "filedb.h"

FileDb::FileDb(): Accessor()
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

void FileDb::deleteFileFromDb(const QString filename)
{
    execQuery(deleteFileQuery(filename));
}

QString FileDb::addFileQuery(const File &file)
{
    return "INSERT INTO File (name) VALUES ('"
            +file.mName + "')";
}

QString FileDb::deleteFileQuery(const QString filename)
{
    return "DELETE FROM User WHERE name = '" + filename + "'";
}

void FileDb::fillStructureFile(File &file)
{
    file.mName = query.record().value(0).toString();
}
