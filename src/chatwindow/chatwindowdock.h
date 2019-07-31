#ifndef CHATWINDOWDOCK_H
#define CHATWINDOWDOCK_H

#include <QDockWidget>
#include "localconnector.h"

class ChatWidget;

class ChatWindowDock: public QDockWidget
{
    Q_OBJECT

    ChatWidget *pChatWidget;
public:
    ChatWindowDock(QWidget *pParent = nullptr);
public slots:
    void updateUsersListOnDiscovery(const LocalConnectorInterface * connector);
};

#endif // CHATWINDOWDOCK_H
