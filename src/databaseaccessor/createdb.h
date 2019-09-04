#ifndef CREATEDB_H
#define CREATEDB_H
#include "accessor.h"

class CreateDB: public Accessor
{
public:
    CreateDB();
    ~CreateDB();
    void addTableUser();
    void addTableMessage();
    void addTableComment();
    void addTableFile();
private:
    QString tableUserQuery();
    QString tableMessageQuery();
    QString tableCommentQuery();
    QString tableFileQuery();
};

#endif // CREATEDB_H
