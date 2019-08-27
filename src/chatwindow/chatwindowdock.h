#ifndef CHATWINDOWDOCK_H
#define CHATWINDOWDOCK_H

#include <QDockWidget>
#include "chatwidgetinterface.h"

// ==========================================================================================
//                                                                          CHAT DOCKER PROXY
// ==========================================================================================
class ChatWindowDock: public QDockWidget
{
    Q_OBJECT

public:

    explicit ChatWindowDock(QWidget *pParent = nullptr);

    // Current user name setter
    void setUserName(const QString &userName);

signals:

    // Signals to GUI that user starts or stops sharing
    // or shares message
    void startSharingWithUser(const QString &);
    void stopSharingWithUser (const QString &);

    void shareMessage(const QString &);

public slots:

    // Slots for processing GUI events
    void pushMessageToChat(const QString userName,
                           const QString message);
    void updateTheme(const QString &newThemeName);

    // Slots for processing Network events
    void updateOnlineUsersOnChange   (const QStringList onlineUsers);
    void updateConnectedUsersOnChange(const QStringList connectedUsers);

private slots:

    // Slots for processing chat events
    void sendRequestOnStartSharing(const QString &userName);
    void sendRequestOnStopSharing (const QString &userName);

    void shareMessageOnSend(const QString &message);

private:

    // Chat instance
    ChatWidgetInterface *mpChatWidget;

    // Set custom behavior to keyPressEvent
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // CHATWINDOWDOCK_H
