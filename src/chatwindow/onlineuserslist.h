#ifndef USERSBACKEND_H
#define USERSBACKEND_H

#include <QObject>

struct OnlineChatUser {
    bool mConnected;
    QString mUserName;
};

class OnlineUsersList : public QObject
{
    Q_OBJECT
public:
    explicit OnlineUsersList(QObject *parent = nullptr);

signals:

public slots:
};

#endif // USERSBACKEND_H
