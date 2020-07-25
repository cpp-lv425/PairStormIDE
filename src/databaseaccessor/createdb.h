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
    static QString tableUserQuery();
    static QString tableMessageQuery();
    static QString tableCommentQuery();
    static QString tableFileQuery();
};

#endif // CREATEDB_H
