#ifndef MESSAGEDB_H
#define MESSAGEDB_H
#include "accessor.h"
#include "structsfordb.h"

class MessageDb : public Accessor
{
public:
    MessageDb();
    void addMessageToDb(const Message& message);
    void getMessageFromDb(const int messageId);
private:
    QString addMessageQuery(const Message& message);
    QString getMessageQuery(const int messageId);
};

#endif // MESSAGEDB_H
