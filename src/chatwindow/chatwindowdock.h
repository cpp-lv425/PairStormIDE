#ifndef CHATWINDOWDOCK_H
#define CHATWINDOWDOCK_H

#include <QDockWidget>

class ChatWidget;

class ChatWindowDock: public QDockWidget
{
    Q_OBJECT

    ChatWidget *mpChatWidget;
public:
    ChatWindowDock(QWidget *pParent = nullptr);

    void setUserName(const QString& userName);
    void displayMessage(const QString userName,
                        const QString message);

signals:
    void userToConnectSelected(const QString);
    void sendMessage(const QString&);

public slots:
    void updateOnlineUsersOnChange(const QStringList onlineUsers);
    void updateConnectedUsersOnChange(const QStringList connectedUsers);
    void onUserToConnectSelected(QString userName);
    void onSendMessage(const QString & message);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CHATWINDOWDOCK_H
