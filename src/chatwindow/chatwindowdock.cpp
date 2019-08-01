#include "chatwindowdock.h"

#include <QListWidgetItem>
#include <QException> // temp
#include <QDebug>

#include "mainwindow.h"
#include "chatwidget.h"

ChatWindowDock::ChatWindowDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle("Chat");
    mpChatWidget = new ChatWidget;
    connect(mpChatWidget, &ChatWidget::userToConnectSelected,
            this, &ChatWindowDock::onUserToConnectSelected);
    setWidget(mpChatWidget);
    setMinimumWidth(200);
}

void ChatWindowDock::setUserName(const QString &userName)
{
    mpChatWidget->setCurrentUserName(userName);
}

void ChatWindowDock::displayMessage(const QString &userName, const QString &message)
{
    mpChatWidget->displayMessage(userName, message);
}

void ChatWindowDock::onUserToConnectSelected(QString userName)
{
    qDebug() << "dock slot";
    emit userToConnectSelected(userName);
}
