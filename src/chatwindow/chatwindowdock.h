#ifndef CHATWINDOWDOCK_H
#define CHATWINDOWDOCK_H

#include <QDockWidget>
#include "chatwidgetinterface.h"

class ChatWidget;

class ChatWindowDock: public QDockWidget
{
    Q_OBJECT

    ChatWidgetInterface * mpChatWidget;

public:
    explicit ChatWindowDock(QWidget *pParent = nullptr);
    void setUserName(const QString& userName);
    void displayMessage(const QString userName,
                        const QString message);
    void updateTheme(const QString & newThemeName);

signals:
    void userToConnectSelected(const QString);
    void userToDisconnectSelected(const QString);

    void sendMessage(const QString&);

public slots:
    void updateOnlineUsersOnChange(const QStringList onlineUsers);
    void updateConnectedUsersOnChange(const QStringList connectedUsers);

    void onUserToConnectSelected(QString userName);
    void onUserToDisconnectSelected(QString userName);

    void onMessageSent(const QString & message);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CHATWINDOWDOCK_H
