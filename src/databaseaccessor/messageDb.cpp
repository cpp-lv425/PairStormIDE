#include "messageDb.h"

MessageDb::MessageDb(): Accessor()
{

}

void MessageDb::addMessageToDb(const Message &message)
{
    execQuery(addMessageQuery(message));
}

void MessageDb::getMessageFromDb(const int messageId)
{
    execQuery(getMessageQuery(messageId));
}

QString MessageDb::addMessageQuery(const Message &message)
{
    return "INSERT INTO Message (idUser, body) VALUES ('"
            + QString::number(message.mIdUser) + "', '"
            + message.mBody + "')";
}

QString MessageDb::getMessageQuery(const int messageId)
{
    return "SELECT * FROM Message WHERE id = "
            + QString::number(messageId);
}
