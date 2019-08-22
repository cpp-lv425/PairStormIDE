#ifndef CHATUSERSCONTROLLER_H
#define CHATUSERSCONTROLLER_H

#include <QObject>
#include <QDateTime>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>

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

class ChatUsersController : public QObject
{
    Q_OBJECT

public:
    explicit ChatUsersController(QObject *parent = nullptr);

    QVector<ChatUser> users() const;

    void sendGreetingsMessage();
    void sendCanNotLogInTwiceMessage();

signals:

    void sendingMessage(const ChatUser & newMessage);
    void preUserAppended();
    void postUserAppended();

public slots:

    void appendUser(const ChatUser & newUser);

    void appendMessage(const QString & newMessageContent);

private:

    QVector<ChatUser> mChatUsers;
};

#endif // CHATUSERSCONTROLLER_H
