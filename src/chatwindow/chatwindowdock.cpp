#include "chatwindowdock.h"
#include "qmlchatwidget.h"
#include "chatwidget.h"
// ==========================================================================================
// ==========================================================================================
//                                                                                CONSTRUCTOR
ChatWindowDock::ChatWindowDock(QWidget *pParent) :
    QDockWidget (pParent)
{
    // Build chat widget & connect it to the dock
    mpChatWidget = new QmlChatWidget;
    setWidget(mpChatWidget);

    connect(mpChatWidget, &ChatWidgetInterface::startSharingRequested,
            this,         &ChatWindowDock::sendRequestOnStartSharing,
            Qt::UniqueConnection);

    connect(mpChatWidget, &ChatWidgetInterface::stopSharingRequested,
            this,         &ChatWindowDock::sendRequestOnStopSharing,
            Qt::UniqueConnection);

    connect(mpChatWidget, &ChatWidgetInterface::messageSent,
            this,         &ChatWindowDock::shareMessageOnSend,
            Qt::UniqueConnection);

    setWindowTitle(mscChatTitle);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                 CONFIGURE CHAT WHEN GUI SETS NEW USER NAME
void ChatWindowDock::setUserName(const QString &userName)
{
    mpChatWidget->configureOnLogin(userName);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                       GIVE RECEIVED MESSAGE TO CHAT WIDGET
void ChatWindowDock::pushMessageToChat(const QString userName, const QString message)
{
    mpChatWidget->appendMessage(userName, message);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                 UPDATE THEME IN THE WIDGET
void ChatWindowDock::updateTheme(const QString &newThemeName)
{
    mpChatWidget->updateTheme(newThemeName);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                   START SHARING WHEN USER INTENDS TO SHARE
void ChatWindowDock::sendRequestOnStartSharing(const QString &userName)
{
    emit startSharingWithUser(userName);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                               STOP SHARING WHEN USER WANTS TO STOP SHARING
void ChatWindowDock::sendRequestOnStopSharing(const QString &userName)
{
    emit stopSharingWithUser(userName);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                  SHARE MESSAGES, OUTGOING FROM CHAT WIDGET
void ChatWindowDock::shareMessageOnSend(const QString &message)
{
    emit shareMessage(message);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                       UPDATE ONLINE USERS LIST IN THE CHAT
void ChatWindowDock::updateOnlineUsersOnChange(const QStringList onlineUsers)
{
    mpChatWidget->updateOnlineUsers(onlineUsers);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                         UPDATE CONNECTED USERS IN THE CHAT
void ChatWindowDock::updateConnectedUsersOnChange(const QStringList connectedUsers)
{
    mpChatWidget->updateConnectedUsers(connectedUsers);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                          PASS KEY PRESS EVENTS TO THE CHAT
void ChatWindowDock::keyPressEvent(QKeyEvent *event)
{
    mpChatWidget->keyPressEvent(event);
    QDockWidget::keyPressEvent(event);
}

const QString ChatWindowDock::mscChatTitle = QString("Chat");
