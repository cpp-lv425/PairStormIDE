#ifndef CHATWINDOWDOCK_H
#define CHATWINDOWDOCK_H

#include <QDockWidget>

class ChatWindowDock: public QDockWidget
{
    QWidget *pChatWidget;
public:
    ChatWindowDock(QWidget *pParent = nullptr);
};

#endif // CHATWINDOWDOCK_H
