#ifndef CHATUSER_H
#define CHATUSER_H

//============================================================ General Qt libraries
#include <QString>


struct ChatUser
{
    QString mUserName;
    enum State : int
    {
        DisconnectedUser,
        ConnectedUser,
        OfflineUser
    } mState;
};

#endif // CHATUSER_H
