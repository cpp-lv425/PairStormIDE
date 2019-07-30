#ifndef CHATWINDOWDOCK_H
#define CHATWINDOWDOCK_H

#include <QDockWidget>

class ChatWidget;

class ChatWindowDock: public QDockWidget
{
    Q_OBJECT

    ChatWidget *pChatWidget;
public:
    ChatWindowDock(QWidget *pParent = nullptr);
};

#endif // CHATWINDOWDOCK_H
