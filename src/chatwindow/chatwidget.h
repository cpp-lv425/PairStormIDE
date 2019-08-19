#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

class OnlineUsersList;
class QListWidgetItem;
class QPlainTextEdit;
class QQuickWidget;
class QQmlContext;
class QListWidget;
class QLineEdit;

class ChatWidget: public QWidget
{
    Q_OBJECT

    QListWidget *mpUsersList;
    QStringList  mOnlineUsers;
    QStringList  mConnectedUsers;
    QString      mUserName;

    QPlainTextEdit *mpFeed;
    QLineEdit      *mpEnterLine;



    QQuickWidget * mChatQuickWidget;
    QQmlContext * mpChatContext;

    OnlineUsersList * mpOnlineUsers;

    void updateUsersList();

public:
    explicit ChatWidget(QWidget *pParent = nullptr);

    void setOnlineUsers(const QStringList & onlineUsers);
    void setConnectedUsers(const QStringList & connectedUsers);

    bool isUserConnected(const QString & userName);

    void setCurrentUserName(const QString& userName);
    void displayMessage(const QString& userName,
                        const QString& message);

public slots:
    void onSendCommand();

private slots:
    void onUserToConnectSelected(QListWidgetItem *item);
    void updateFeedOnSend();

signals:
    void userToConnectSelected(QString);
    void sendMessage(const QString&);
};

#endif // CHATWIDGET_H
