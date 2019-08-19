#ifndef USERSBACKEND_H
#define USERSBACKEND_H

#include <QObject>
#include <QVector>

struct OnlineChatUser {
    QString mUserName;
    bool mConnected;
};

class OnlineUsersList : public QObject
{
    Q_OBJECT
public:
    explicit OnlineUsersList(QObject *parent = nullptr);

    QVector<OnlineChatUser> users() const;

signals:
    void preUserAppeared();
    void postUserAppeared();

    void preUserDisappeared(int index);
    void postUserDisappeared();

public slots:

    void updateOnlineUsersOnChanges(const QStringList & userNames);
    void updateConnectedUsersOnChanges(const QStringList & userNames);
    void connectToUserOnClick();

private:
    QVector<OnlineChatUser> mOnlineChatUsers;
};

#endif // USERSBACKEND_H
