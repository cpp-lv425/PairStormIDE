#include "chatwindowdock.h"

#include <QListWidgetItem>
#include <QException> // temp
#include <QDebug>

#include "mainwindow.h"
#include "chatwidget.h"

ChatWindowDock::ChatWindowDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle("Chat");
    pChatWidget = new ChatWidget;
    connect(pChatWidget, &ChatWidget::userToConnectSelected,
            this, &ChatWindowDock::onUserToConnectSelected);
    setWidget(pChatWidget);
    setMinimumWidth(200);
}

void ChatWindowDock::onUserToConnectSelected(QString userName)
{
    qDebug() << "dock slot";
    emit userToConnectSelected(userName);
}
