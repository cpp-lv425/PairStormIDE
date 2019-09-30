 #ifndef FILEDB_H
#define FILEDB_H
#include "accessor.h"
#include "structsfordb.h"

class FileDb : public Accessor
{
public:
    FileDb();
    ~FileDb();
    void addFileToDb(const File &file);
    void deleteFileFromDb(const QString filename);
private:
    static QString addFileQuery(const File &file);
    static QString deleteFileQuery(const QString filename);
    void fillStructureFile(File &file);

};

#endif // FILEDB_H
