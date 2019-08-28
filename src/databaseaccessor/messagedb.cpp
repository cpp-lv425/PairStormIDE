#include "messagedb.h"

MessageDb::MessageDb(): Accessor()
{
}

MessageDb::~MessageDb()
{
    query.finish();
}

void MessageDb::addMessageToDb(const Message &message)
{
    execQuery(addMessageQuery(message));
}

QVector<Message> MessageDb::getMessageFromDb(const QString startTime)
{
    execQuery(numberOfMessages(startTime));
    int count_of_messages =query.value(0).toInt();
    QVector<Message> messages(count_of_messages);
    execQuery(getMessageQuery(startTime));
    int counter = 0;
    while (query.next()) {
        fillStructMessage(messages[counter]);
        counter++;
    }
    return messages;
}

QString MessageDb::addMessageQuery(const Message &message)
{
    return "INSERT INTO Message (idUser, messageText, time) VALUES ("
            "(Select id from User where nickname = '" + message.mUser + "'), '"
            + message.mBody + "', '" + message.mTime + "')";
}

QString MessageDb::getMessageQuery(const QString startTime)
{
    return "Select Message.messageText, User.nickname, datetime(Message.time)"
           " from Message inner join User on Message.idUser = User.id"
           " where date(Message.time) >= '" + startTime +"'";
}

QString MessageDb::numberOfMessages(const QString startTime)
{
    return "Select count(Message.messageText)"
           " from Message inner join User on Message.idUser = User.id"
           " where datetime(Message.time) >= '" + startTime +"'";
}

void MessageDb::fillStructMessage(Message message)
{
    message.mBody = query.record().value(0).toString();
    message.mUser = query.record().value(1).toString();
    message.mTime = query.record().value(2).toString();
}
