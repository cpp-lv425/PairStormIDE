#include "chatwindowdock.h"

#include <QException> // temp

#include "mainwindow.h"

ChatWindowDock::ChatWindowDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle("Chat");
    pChatWidget = new QWidget;
    setWidget(pChatWidget);
    auto pMainWindow = qobject_cast<MainWindow*>(pParent);

    if(!pMainWindow)
        throw QException();

    pMainWindow->addDockWidget(Qt::RightDockWidgetArea, this);
}
