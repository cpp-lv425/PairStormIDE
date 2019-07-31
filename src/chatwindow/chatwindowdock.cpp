#include "chatwindowdock.h"

#include <QException> // temp

#include "mainwindow.h"
#include "chatwidget.h"

ChatWindowDock::ChatWindowDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle("Chat");
    pChatWidget = new ChatWidget;
    setWidget(pChatWidget);
}



void ChatWindowDock::updateUsersListOnDiscovery(const LocalConnectorInterface * connector)
{
    QStringList updatedUsersList(connector->getOnlineUsers());
    pChatWidget->setUsersList(updatedUsersList);
}
