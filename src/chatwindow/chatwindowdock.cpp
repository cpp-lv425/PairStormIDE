#include "chatwindowdock.h"
#include "qmlchatwidget.h"

#include <QListWidgetItem>
#include <QKeyEvent>
#include <QDebug>

#include "mainwindow.h"
#include "chatwidget.h"

ChatWindowDock::ChatWindowDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle("Chat");
    mpChatWidget = new ChatWidget;

    connect(mpChatWidget, &ChatWidget::startSharingRequested,
            this,         &ChatWindowDock::onUserToConnectSelected);

    connect(mpChatWidget, &ChatWidget::stopSharingRequested,
            this,         &ChatWindowDock::onUserToDisconnectSelected);

    connect(mpChatWidget, &ChatWidget::messageSent,
            this,         &ChatWindowDock::onMessageSent);

    mpChatWidget->setMinimumWidth(pParent->width() / 2);
    setWidget(mpChatWidget);
    setDisabled(true);
}

void ChatWindowDock::setUserName(const QString &userName)
{
    mpChatWidget->configureOnLogin(userName);
    setDisabled(false);
}

void ChatWindowDock::displayMessage(const QString userName, const QString message)
{
    mpChatWidget->appendMessage(userName, message);
}

void ChatWindowDock::onUserToConnectSelected(QString userName)
{
    emit userToConnectSelected(userName);
}

void ChatWindowDock::onUserToDisconnectSelected(QString userName)
{
    emit userToDisconnectSelected(userName);
}

void ChatWindowDock::onMessageSent(const QString &message)
{
    emit sendMessage(message);
}

void ChatWindowDock::keyPressEvent(QKeyEvent *event)
{
    mpChatWidget->keyPressEvent(event);
    QDockWidget::keyPressEvent(event);
}

void ChatWindowDock::updateOnlineUsersOnChange(const QStringList onlineUsers)
{
    mpChatWidget->updateOnlineUsers(onlineUsers);
}

void ChatWindowDock::updateConnectedUsersOnChange(const QStringList connectedUsers)
{
    mpChatWidget->updateConnectedUsers(connectedUsers);
}
