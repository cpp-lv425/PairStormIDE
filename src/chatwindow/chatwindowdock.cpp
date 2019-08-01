#include "chatwindowdock.h"

#include <QListWidgetItem>
#include <QKeyEvent>

#include "mainwindow.h"
#include "chatwidget.h"

ChatWindowDock::ChatWindowDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle("Chat");
    mpChatWidget = new ChatWidget;

    connect(mpChatWidget, &ChatWidget::userToConnectSelected,
            this, &ChatWindowDock::onUserToConnectSelected);
    connect(mpChatWidget, &ChatWidget::sendMessage,
            this, &ChatWindowDock::onSendMessage);

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
    emit userToConnectSelected(userName);
}

void ChatWindowDock::onSendMessage(const QString &userName, const QString &message)
{
    emit sendMessage(userName, message);
}

void ChatWindowDock::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter
       || event->key() == Qt::Key_Return)
    {
        mpChatWidget->onSendCommand();
    }

    QDockWidget::keyPressEvent(event);
}
