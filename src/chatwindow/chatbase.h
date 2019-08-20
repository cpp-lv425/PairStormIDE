#ifndef CHATBASE_H
#define CHATBASE_H

#include <QDateTime>

struct ChatUser {
    QString mUserName;
    enum State : int
    {
        DisconnectedUser,
        ConnectedUser
    } mState;
};

struct ChatMessage {
    QString   mAuthorName;
    QString   mContent;
    QDateTime mDateTime;
};

#endif // CHATBASE_H
