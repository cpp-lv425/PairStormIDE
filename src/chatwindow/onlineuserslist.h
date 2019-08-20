#ifndef USERSBACKEND_H
#define USERSBACKEND_H

#include <QObject>
#include <QVector>
#include "chatbase.h"

class OnlineUsersList : public QObject
{
    Q_OBJECT

public:

    explicit OnlineUsersList(QObject *parent = nullptr);

    ChatUser & at(const int & index);
    int size() const;

signals:

public slots:

    void updateUsers(const QStringList & userNames);
    void connectUsers(const QStringList & userNames);

private:
    QVector<ChatUser> mChatUsers;
};

#endif // USERSBACKEND_H
