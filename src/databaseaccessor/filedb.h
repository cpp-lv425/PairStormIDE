#ifndef FILEDB_H
#define FILEDB_H
#include "accessor.h"
#include "structsfordb.h"

class FileDb : public Accessor
{
public:
    FileDb();
    void addFileToDb(const File& file);
    void getFileFromDb(const int idFile);
    void deleteFileFromDb(const  QString filename);
private:
    QString addFileQuery(const File & file);
    QString getFileQuery(const int idFile);
    QString deleteFileQuery(const QString filename);
};

#endif // FILEDB_H
