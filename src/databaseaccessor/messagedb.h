#ifndef MESSAGEDB_H
#define MESSAGEDB_H
#include "accessor.h"
#include "structsfordb.h"

class MessageDb : public Accessor
{
public:
    MessageDb();
    ~MessageDb();
    void addMessageToDb(const Message& message);
    QVector <Message> getMessageFromDb(const QString startTime);
private:
    QString addMessageQuery(const Message& message);
    QString getMessageQuery(const QString startTime);
    QString numberOfMessages(const QString startTime);
    void fillStructMessage(Message &message);
};

#endif // MESSAGEDB_H
